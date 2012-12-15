/*
 thumb.c
 
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
#include "thumb.h"
#include "thumb_db.h"
#include "common.h"

#include <memory.h>

static unsigned short read_short(const unsigned char* ptr, _EndianityType endianity)
{
#ifdef __BIG_ENDIAN__
    if (endianity == ENDIANITY_BIG) return *(unsigned short*)ptr;
    
    /* ENDNIANITY_LITTLE. */
    return return (ptr[0] << 8) | ptr[1];
#else
    if (endianity == ENDIANITY_BIG) return (ptr[0] << 8) | ptr[1];
    
    /* ENDNIANITY_LITTLE. */
    return *(unsigned short*)ptr;
#endif
}

static const _ThumbInstInfo* decompose_thumb_lookup_instruction(unsigned short instruction, unsigned short nextInstruction)
{
    /* 10 MS bits are the index into the table. Calculate how much to shift right. */
    const unsigned int SHIFT = (sizeof(short) * 8) - 10;
    unsigned int indexer = instruction >> SHIFT;
    /* A single look up to find the instruction! o(1) */
    indexer = ThumbsIds[indexer];
    if (indexer != 0xff) {
        return &ThumbInstructions[indexer];
    }

    /* Is long branch of BL/BLX prefix ? */
    indexer = instruction >> 11; /* Get highest 5 bits. */
    if (indexer == 0xf000) {
        if (indexer == 0xe800) { /* BLX suffix. */
            if (nextInstruction & 1) return NULL; // If LSB is set, it's undefined.
            static _ThumbInstInfo ThumbInfo_BLX = {I_BLX, FLAG_BIG_INST, OT_OFF11, OT_NONE, OT_NONE};
            return &ThumbInfo_BLX;
        }
        if (indexer == 0xf800) { /* BL suffix. */
            static _ThumbInstInfo ThumbInfo_BL = {I_BL, FLAG_BIG_INST, OT_OFF11, OT_NONE, OT_NONE};
            return &ThumbInfo_BL;
        }
    }
    
    // TBD: Check for special opcode.
    
    return NULL;
}

static int decompose_thumb_operand(unsigned short instruction, unsigned short nextInstruction, _OpType oType, unsigned int* opIndex, _DInst* pdi)
{
    /* The reason we use an indexer is because some operands are optional and we want to skip them. */
    _Operand* op = &pdi->operands[*opIndex];
    (*opIndex) += 1;
    
    switch (oType)
    {
        case OT_RD:
        case OT_RN0:
            op->type = OPERAND_REG;
            op->value = instruction & 7;
            break;
        case OT_RD8:
        case OT_RN8:
            op->type = OPERAND_REG;
            op->value = (instruction >> 8) & 7;
            break;
        case OT_HRD:
            op->type = OPERAND_REG;
            op->value = ((instruction & 0x80) >> 4) | (instruction & 7);
            break;
        case OT_RN3:
        case OT_RM3:
        case OT_RS3:
            op->type = OPERAND_REG;
            op->value = (instruction >> 3) & 7;
            break;
        case OT_HRN0:
            op->type = OPERAND_REG;
            op->value = ((instruction & 0x80) >> 4) | (instruction & 7);
            break;
        case OT_RM6:
            op->type = OPERAND_REG;
            op->value = (instruction >> 6) & 7;
            break;
        case OT_HRM3:
            op->type = OPERAND_REG;
            op->value = (instruction >> 3) & 0xf;
            break;
        case OT_IMM3_6:
            /* Support hack for 0001110 ADD RD RN3 IMM3_6 ***HACK:IF IMM3 OPCODE is MOV. */
            op->value = (instruction >> 6) & 7;
            if ((pdi->opcode == I_ADD) && (op->value == 0)) {
                pdi->opcode = I_MOV;
                /* We don't need this immediate then (so it won't be: MOV RX, RY, #n). */
                break;
            }
            op->type = OPERAND_IMM;
            break;
        case OT_IMM5:
            op->type = OPERAND_IMM;
            op->value = instruction & 0x1f;
            break;
        case OT_IMM5_6:
            op->type = OPERAND_IMM;
            op->value = (instruction >> 6) & 0x1f;
            break;
        case OT_IMM7:
            op->type = OPERAND_IMM;
            op->value = instruction & 0x7f;
            break;
        case OT_IMM8:
            op->type = OPERAND_IMM;
            op->value = instruction & 0xff;
            break;
        case OT_OFF8:
            /* Signed immediate 8 bits is used for short branches. */
            op->type = OPERAND_ADDR;
            op->value = instruction & 0xff;
            pdi->target = (char)op->value;
            pdi->target += pdi->address + 4;
            pdi->flags |= FLAG_SIGNED_IMM;
            break;
        case OT_OFF11: {
            unsigned int simm = (instruction & 0x7ff);
            /*
             1) Shift left simm by 1.
             2) Sign extend off to 32. */
            int off = (simm << 21);
            /* (1,2 are optimized with arithmetic shift right). */
            off >>= 20;
            /* Fix for a long BL/BLX offset. */
            if (pdi->flags & FLAG_BIG_INST) {
                /* 3) Add nextSimm << 2; */
                off += (nextInstruction & 0x7ff) << 2;
            }
            op->type = OPERAND_ADDR;
            /* 4) Add PC + 4. */
            pdi->target = pdi->address + off + 4;
        } break;
        case OT_SP:
            op->type = OPERAND_REG;
            op->value = REG_SP;
            break;
        case OT_PC8:
            if (~instruction & 0x100) {
                (*opIndex)--;
                break; /* Exit if bit 8 isn't set. */
            }
            /* FALL DOWN! */
        case OT_PC:
            op->type = OPERAND_REG;
            op->value = REG_PC;
            break;
        case OT_LR8:
            if (~instruction & 0x100) {
                (*opIndex)--;
                break; /* Exit if bit 8 isn't set. */
            }
            /* FALL DOWN! */
        case OT_LR:
            op->type = OPERAND_REG;
            op->value = REG_LR;
            break;
        case OT_REGLIST:
            op->type = OPERAND_REGLIST;
            op->value = instruction & 0xff;
            break;
        case OT_SBZ3_0: /* Should-Be-Zero 3 bits at 0. */
            if ((instruction & 7) != 0) {
                (*opIndex)--;
                return 0;
            }
            break;
        case OT_NONE:
        default:
            (*opIndex)--;
            /* Ignore. */
            break;
    }
    
    /* Fix scale for immediates for STR/LD opcodes. */
    if ((pdi->flags & FLAG_SCALE_REQ) && (op->type == OPERAND_IMM)) {
        int scale;
        switch (pdi->opcode)
        {
            case I_STRH:
            case I_LDRH:
                scale = 2;
                break;
            case I_STR:
            case I_LDR:
                scale = 4;
                break;
            default:
                scale = 1;
        }
        op->value *= scale;
    }
    
    return 1;
}

static int decompose_thumb_instruction(unsigned short instruction, unsigned short nextInstruction, _DInst* pdi)
{
    int ret;
    unsigned int operandIndex = 0;
    const _ThumbInstInfo* ii;
    
    ii = decompose_thumb_lookup_instruction(instruction, nextInstruction);
    if (ii == NULL) {
        /* Instruction wasn't found. */
        return 0;
    }
    pdi->opcode = ii->mnemonicId;
    pdi->flags = ii->flags;
    
    /* If there's an operand to decode, go for it, otherwise quit. */
    
    if (ii->op1 != OT_NONE) {
        ret = decompose_thumb_operand(instruction, nextInstruction, ii->op1, &operandIndex, pdi);
        if (!ret) return 0;
    } else return 1;
    
    if (ii->op2 != OT_NONE) {
        ret = decompose_thumb_operand(instruction, nextInstruction, ii->op2, &operandIndex, pdi);
        if (!ret) return 0;
    } else return 1;
    
    if (ii->op3 != OT_NONE) {
        ret = decompose_thumb_operand(instruction, nextInstruction, ii->op3, &operandIndex, pdi);
        if (!ret) return 0;
    }
    
    return 1;
}

_DecodeResult decompose_thumb(_DecomposeInfo* info)
{
    unsigned int size;
    unsigned int instsNo = 0;
    unsigned short instruction, nextInstruction;
    _DInst* pdi;
    
    while ((info->codeLength > 0) && (instsNo < info->maxInstructions)) {
        /* Read current instruction from stream. */
        instruction = read_short(info->code, info->endianity);
        
        /* Try to read next instruction from stream, used for long branches. */
        if ((info->codeLength - 2) >= 2) {
            nextInstruction = read_short(info->code + 2, info->endianity);
        } else {
            nextInstruction = 0;
        }
        
        pdi = &info->instructions[instsNo];
        memset((void*)pdi, 0, sizeof(_DInst));
        pdi->address = info->address;
        if (!decompose_thumb_instruction(instruction, nextInstruction, pdi)) {
            pdi->flags = FLAG_INVALID;
            pdi->opcode = I_UNDEFINED;
            /* Opcode wasn't found. */
        }
        
        /* BL/BLX could be 4 bytes. */
        size = (pdi->flags & FLAG_BIG_INST) ? 4 : 2;
        instsNo += 1;
        info->address += size;
        info->code += size;
        info->codeLength -= size;
    }
    
    /* Update number of decoded instructions. */
    info->decodedInstructionsCount = instsNo;
    
    return DECRES_SUCCESS;
}
