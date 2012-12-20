/*
 defs.c
 
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

const unsigned char _THUMB_MNEMONICS[] =
"\x09" "UNDEFINED\0" "\x02" "BL\0" "\x03" "LSL\0" "\x03" "LSR\0" "\x03" "ASR\0" \
"\x03" "MOV\0" "\x03" "CMP\0" "\x03" "ADD\0" "\x03" "SUB\0" "\x03" "LDR\0" \
"\x03" "STR\0" "\x04" "STRB\0" "\x04" "LDRB\0" "\x04" "STRH\0" "\x04" "LDRH\0" \
"\x05" "STMIA\0" "\x05" "LDMIA\0" "\x01" "B\0" "\x05" "LDRSB\0" "\x05" "LDRSH\0" \
"\x04" "PUSH\0" "\x03" "POP\0" "\x03" "BEQ\0" "\x03" "BNE\0" "\x03" "BCS\0" \
"\x03" "BCC\0" "\x03" "BMI\0" "\x03" "BPL\0" "\x03" "BVS\0" "\x03" "BVC\0" \
"\x03" "BHI\0" "\x03" "BLS\0" "\x03" "BGE\0" "\x03" "BLT\0" "\x03" "BGT\0" \
"\x03" "BLE\0" "\x03" "BAL\0" "\x03" "SWI\0" "\x04" "BKPT\0" "\x02" "BX\0" \
"\x03" "BLX\0" "\x03" "AND\0" "\x03" "EOR\0" "\x03" "ADC\0" "\x03" "SBC\0" \
"\x03" "ROR\0" "\x03" "TST\0" "\x03" "NEG\0" "\x03" "CMN\0" "\x03" "ORR\0" \
"\x03" "MUL\0" "\x03" "BIC\0" "\x03" "MVN\0" "\x04" "SXTH\0" "\x04" "SXTB\0" \
"\x04" "UXTH\0" "\x04" "UXTB\0" "\x03" "REV\0" "\x05" "REV16\0" "\x05" "REVSH\0";

char* _REGISTERS[] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC"};
