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

/* Some flags are defined in armstorm.h and are useable by the user. */

/* Use this mask to filter the flags that the user can't see. */
#define FLAGS_EXPORTED_MASK (0xf)

/* The following are not exported to the user and are internal only. */

/* Immediate requires scale according to opcode. */
#define FLAG_SCALE_REQ  0x10

unsigned short read_short(const unsigned char* ptr, _EndianityType endianity);
unsigned long read_long(const unsigned char* ptr, _EndianityType endianity);

#endif /* armstorm_defs_h */
