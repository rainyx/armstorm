//
//  main.c
//  distorm_arm
//
//  Created by Gil Dabah on 12/7/12.
//  Copyright (c) 2012 Gil Dabah. All rights reserved.
//

// 1) Tests
// 2) Documentation

#include "armstorm.h"
#include <stdio.h>

int main(int argc, const char * argv[])
{
    unsigned char buf[] = {0xe7, 0xfe};//, 0xc5, 0xc0, 0x1c, 0x33, 0xbd, 0xff, 0x4c, 0x1c, 0xb5, 0x70, 0x4c, 0x1c, 0xf3, 0xaf, 0x80, 0x00, 0xbf, 0x00, 0x46, 0xc0, 0, 0, 0xbd, 0xff};
    _DInst insts[10] = {0};
    _DecomposeInfo info = {0};
    info.address = 1;
    info.code = buf;
    info.codeLength = sizeof(buf);
    info.endianity = ENDIANITY_BIG;
    info.maxInstructions = 10;
    info.instructions = insts;
    armstorm_decompose(&info);
    for (int i = 0; i < info.decodedInstructionsCount; i++) {
        _TInst t = {0};
        armstorm_format(&insts[i], &t, ENDIANITY_BIG);
        printf("%s\n", t.instruction);
    }
    return 0;
}

