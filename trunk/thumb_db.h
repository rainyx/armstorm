/*
 thumb_db.h
 
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

#ifndef armstorm_thumb_db_h
#define armstorm_thumb_db_h

typedef struct {
    unsigned short mnemonicId;
    unsigned char flags;
    unsigned char op1, op2, op3;
} _ThumbInstInfo;

typedef enum {
	OT_NONE,
	OT_RD,
	OT_RD8,
	OT_HRD,
	OT_RN0,
	OT_RN3,
	OT_RN8,
	OT_HRN0,
	OT_RM3,
	OT_RM6,
	OT_HRM3,
	OT_RS3,
	OT_IMM3_6,
	OT_IMM5,
	OT_IMM5_6,
	OT_IMM7,
	OT_IMM8,
	OT_OFF8,
	OT_OFF11,
	OT_SP,
	OT_PC,
	OT_PC8,
	OT_LR,
	OT_LR8,
	OT_REGLIST,
	OT_SBZ3_0,
} _OpType;

extern _ThumbInstInfo ThumbInstructions[];
extern unsigned char ThumbsIds[];

#endif /* armstorm_thumb_db_h */
