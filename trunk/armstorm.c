/*
 armstorm.c
 
 http://code.google.com/p/armstorm/
 distorm at gmail dot com
 Copyright (C) 2012 Gil Dabah
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "armstorm.h"
#include "common.h"
#include "thumb.h"
#include "arm.h"

#include <stdio.h>
#include <memory.h>

// TBD: Finish formatting instruction to _TInst.

_DecodeResult armstorm_decompose(_DecomposeInfo* info)
{
    if (info->code == NULL) return DECRES_INPUTERR;
    if ((info->endianity != ENDIANITY_BIG) && (info->endianity != ENDIANITY_LITTLE)) return DECRES_INPUTERR;
    if (info->instructions == NULL) return DECRES_INPUTERR;
    
    /* If the address is odd, we will decode THUMB instructions. */
    if (info->address & 1) {
        if ((info->codeLength % 2) != 0) return DECRES_INPUTERR;
        return decompose_thumb(info);
    }
    
    /* The address is even - decode ARM instructions. */
    if ((info->codeLength % 4) != 0) return DECRES_INPUTERR;
    return decompose_arm(info);
}

static void format_reglist(char* str, unsigned int mask)
{
    int backup = mask;
    int base = 0;
    int runLength = 0;
    while (!(mask & 1)) base++, mask >>= 1;
    while (mask & 1) runLength++, mask >>= 1;
    if (!mask && runLength > 2) {
        char buf[20];
        sprintf(buf, "R%d-R%d", base, base + runLength - 1);
        strcat(str, buf);
    } else {
        int prev = 0;
        mask = backup;
        for (int j = 0; mask; j++) {
            if (mask & (1 << j)) {
                if (prev) strcat(str, ", ");
                strcat(str, (const char*)_REGISTERS[j]);
                mask ^= (1 << j);
                prev = 1;
            }
        }
    }
}

static void format_operand(char* str, const _Operand* op, const _DInst* pdi)
{
    char buf[128];
    switch (op->type)
    {
        case OPERAND_REG:
            strcat(str, GET_REGISTER_NAME(op->value));
            break;
        case OPERAND_IMM:
            sprintf(buf, "#0x%x", op->value);
            strcat(str, buf);
            break;
        case OPERAND_ADDR:
            sprintf(buf, "0x%llx", pdi->target);
            strcat(str, buf);
            break;
        case OPERAND_REGLIST:
        {
            unsigned char mask = op->value;
            strcat(str, "{");
            if (mask) {
                format_reglist(str, mask);
            }
        } break;
    }
}

void armstorm_format(const _DecomposeInfo* ci, const _DInst* inst, _TInst* text)
{
    int isReglist = 0, needComma = 1;
    char* str = text->instruction;

    /* Fill in address. */
    text->address = inst->address;
    
    /* Fill in the size and the hex representation. */
    if (inst->flags & FLAG_BIG_INST) {
        text->size = 4;
        sprintf(text->hex, "%08lx", read_long(&ci->code[inst->address - ci->address], ci->endianity));
    } else {
        text->size = 2;
        sprintf(text->hex, "%04x", read_short(&ci->code[inst->address - ci->address], ci->endianity));
    }

    if (inst->flags == FLAG_INVALID) {
        strcpy(str, "UNDEFINED");
        return;
    }

    /* Start with the mnemonic for the instruction. */
    strcpy(str, GET_MNEMONIC_NAME(inst->opcode));
    strcat(str, " ");

    /* For now we know there's always at least a single operand. */
    format_operand(str, &inst->operands[0], inst);

    if ((inst->opcode == I_LDMIA || inst->opcode == I_STMIA)) {
        strcat(str, "!");
        isReglist = 1;
    } else if ((inst->opcode == I_PUSH) || (inst->opcode == I_POP)) isReglist = 1;

    /* If the instruction touches memory, after first operand use brackets. */
    if (inst->flags & FLAG_MEMORY) {
        strcat(str, ", [");
        needComma = 0;
    }

    /* Add the rest of the operands. */
    for (unsigned int i = 1; ((inst->operands[i].type != OPERAND_NONE) && (i < 3)); i++) {
        if (needComma) strcat(str, ", ");
        format_operand(str, &inst->operands[i], inst);
        needComma = 1;
    }

    /* Append brackets if required. */
    if (isReglist) strcat(str, "}");
    if (inst->flags & FLAG_MEMORY) strcat(str, "]");
}
