/*
 armstorm - ARM/THUMB Disassembler Library

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

#ifndef armstorm_h
#define armstorm_h

#define ARMSTORM_VERSION 0.1

typedef enum {ENDIANITY_BIG, ENDIANITY_LITTLE} _EndianityType;
typedef unsigned long long _AddressType;
typedef enum {OPERAND_NONE, OPERAND_REG, OPERAND_IMM, OPERAND_ADDR, OPERAND_REGLIST} _OperandType;
typedef enum {DECRES_SUCCESS, DECRES_INPUTERR} _DecodeResult;

typedef enum {
    REG_R0 = 0,
    REG_R1,
    REG_R2,
    REG_R3,
    REG_R4,
    REG_R5,
    REG_R6,
    REG_R7,
    REG_R8,
    REG_R9,
    REG_R10,
    REG_R11,
    REG_R12,
    REG_SP,
    REG_LR,
    REG_PC
} _ARM_REGISTERS;

typedef enum {
	I_UNDEFINED = 0, I_ADC = 231, I_ADD = 40, I_AND = 221, I_ASR = 25, I_B = 98, I_BAL = 196, I_BCC = 141,
	I_BCS = 136, I_BEQ = 126, I_BGE = 176, I_BGT = 186, I_BHI = 166, I_BIC = 271,
	I_BKPT = 206, I_BL = 11, I_BLE = 191, I_BLS = 171, I_BLT = 181, I_BLX = 216,
	I_BMI = 146, I_BNE = 131, I_BPL = 151, I_BVC = 161, I_BVS = 156, I_BX = 212,
	I_CMN = 256, I_CMP = 35, I_EOR = 226, I_LDMIA = 91, I_LDR = 50, I_LDRB = 66,
	I_LDRH = 78, I_LDRSB = 101, I_LDRSH = 108, I_LSL = 15, I_LSR = 20, I_MOV = 30,
	I_MUL = 266, I_MVN = 276, I_NEG = 251, I_ORR = 261, I_POP = 121, I_PUSH = 115,
	I_REV = 305, I_REV16 = 310, I_REVSH = 317, I_ROR = 241, I_SBC = 236, I_STMIA = 84,
	I_STR = 55, I_STRB = 60, I_STRH = 72, I_SUB = 45, I_SWI = 201, I_SXTB = 287,
	I_SXTH = 281, I_TST = 246, I_UXTB = 299, I_UXTH = 293
} _ThumbInstructionType;

typedef struct {
    /* __OUT */ unsigned char type;  /* Type of operand from _OperandType. */
    /* __OUT */ short value;         /* Value of operand according to its type. */
} _Operand;

/* Flags type for an instruction. */
typedef unsigned char _iflags;

/* Instruction wasn't found or has an invalid operand. */
#define FLAG_INVALID    1
/* One of the load or store instructions. */
#define FLAG_MEMORY     2
/* The big inst means it's 4 bytes long instruction, otherwise 2. */
#define FLAG_BIG_INST   4
/* Immediate operand is be sign extended. */
#define FLAG_SIGNED_IMM 8

typedef struct {
    /* __OUT */ _AddressType   address;     /* Address of current instruction. */
    /* __OUT */ _AddressType   target;      /* Target address of a branch instruction. */
    /* __OUT */ unsigned short opcode;      /* The opcode id in _ThumbOpcodeType. */
    /* __OUT */ _iflags        flags;       /* See flags above. */
    /* __OUT */ _Operand       operands[3]; /* Up to three operands of struct _Operand. */
} _DInst;

typedef struct {
    /* __IN */ _AddressType         address;         /* An ODD ADDRESS will cause to decode THUMB instructions! */
    /* __IN */ const unsigned char* code;            /* Pointer to code bytes. (Should be aligned to 2/4). */
    /* __IN */ unsigned int         codeLength;      /* Must be even, a multiple of 2/4 depends on mode. */
    /* __IN */ _EndianityType       endianity;       /* Decode instructions in big/little endian. */
    /* __IN */ unsigned int         maxInstructions; /* Number of instructions that can be written to 'instructions'. */

    /* __OUT */ _DInst*      instructions ;            /* Decoded instructions are written to 'instructions'. */
    /* __OUT */ unsigned int decodedInstructionsCount;  /* Number of decoded instructions. */
} _DecomposeInfo;

#define GET_REGISTER_NAME(r) ((const char*)_REGISTERS[(r)])
extern char* _REGISTERS[];

#define GET_MNEMONIC_NAME(m) ((const char*)(&_THUMB_MNEMONICS[(m + 1)]))
extern const unsigned char _THUMB_MNEMONICS[];

extern _DecodeResult armstorm_decompose(_DecomposeInfo* info);

typedef struct {
    _AddressType address;
    unsigned char size;
    char hex[11];
    char instruction[40];
} _TInst;

extern void armstorm_format(const _DecomposeInfo* ci, const _DInst* inst, _TInst* text);

#endif /* armstorm_h */
