#ifndef ARITHMETICOPERATION_H
#define ARITHMETICOPERATION_H

union Memory_4{
    unsigned char chr[4];
    int val;
    float valf;
};

/*
Doing arithmetic calculations
@param pos current position of stack pointer, which points to the command, passed as reference to move the pointer
*/
void arithmeticOp(int&pos);

#endif
