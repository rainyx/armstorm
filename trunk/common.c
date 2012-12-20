//
//  common.c
//  distorm_arm
//
//  Created by Gil Dabah on 12/20/12.
//  Copyright (c) 2012 Gil Dabah. All rights reserved.
//

#include "armstorm.h"

unsigned short read_short(const unsigned char* ptr, _EndianityType endianity)
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

unsigned long read_long(const unsigned char* ptr, _EndianityType endianity)
{
    //endianity;
    return *(unsigned long*)ptr;
}
