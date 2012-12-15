/*
 common.h
 
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

#ifndef armstorm_defs_h
#define armstorm_defs_h

/* Instruction wasn't found or has an invalid operand. */
#define FLAG_INVALID    1
/* Immediate should be sign extended. */
#define FLAG_SIGNED_IMM 2
/* NOT SET: THUMB = 2. SET: ARM = 4, THUMB_BL = 4! */
#define FLAG_BIG_INST   4
/* Immediate requires scale according to opcode. */
#define FLAG_SCALE_REQ  8
/* Load or store. */
#define FLAG_MEMORY     16

#endif /* armstorm_defs_h */
