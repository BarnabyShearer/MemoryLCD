/*
 * Miniamlist Arduino library wrapper
 *
 * Copyright 2014 <b@Zi.iS>
 * License GPLv2
 */

#include "Arduino.h"

void delayMicroseconds(const uint32_t t) {
    uint32_t tt = t * 5;
    while(tt-- > 0) {
        asm("");
    }
}

void delay(uint32_t mTime) {
    delayMicroseconds(mTime * 1000);
}
