#pragma once

union Memory_4{
    unsigned char chr[4];
    int val;
    float valf;
};

void arithmeticOp(int&pos,unsigned char oper);
