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
void add(int&next);
void sub(int&next);
void mul(int&next);
void div(int&next);
void rem(int&next);
void and_(int&next);
void or_(int&next);
void fadd(int&next);
void fsub(int&next);
void fmul(int&next);
void fdiv(int&next);

#endif
