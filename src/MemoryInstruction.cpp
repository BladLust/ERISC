//
//  main.cpp
//  ERISC
//
//  Created by Wu Weiyan on 11/27/20.
//

#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
//#include "GeneralFunctions.h"
#include "MemoryInstruction.h"
#include "InstructionParser.h"
#include "arithmeticOperation.h"
#include "drawBitmapImage.h"
#include "outputTxt.h"
bool bln = true;
unsigned char a0, a1, a2, a3;
int imageFileNum;

Function funcPointer[0xff];

void monitor(std::ostream&os=std::cout){
    os<<'['<<simStorage.registers[0];
    for(int i=1;i<32;i++)
	os<<','<<std::hex<<simStorage.registers[i];
    os<<']'<<std::endl;
}

void fmonitor(std::ostream&os=std::cout){
    os<<'['<<simStorage.registers[0];
    for(int i=1;i<32;i++)
	os<<','<<*(float*)(void*)(simStorage.registers+i);
    os<<']'<<std::endl;
}

// function which converts four unsigned char to an integer
int convertToInt(unsigned char a0, unsigned char a1, unsigned char a2,
                 unsigned char a3) {
  unsigned char a[4];
  a[0] = a0;
  a[1] = a1;
  a[2] = a2;
  a[3] = a3;
  int x;
  x = (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
  return x;
}

void monitorMemory(std::ostream&os=std::cout){
    for(int i=0;i<0x400000;i++)
	if(simStorage.memory[i]!=0)
	    os<<std::hex<<'('<<i<<','<<(uint)simStorage.memory[i]<<')'<<' ';
    os<<std::endl;
}

void load(int&next){
    int addrInRs;
    addrInRs=simStorage.registers[instructionStack->stack[next + 2]]; // address in rs
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    uint*ptr=(uint*)((void*)(simStorage.memory+addrInRs));
    simStorage.registers[instructionStack->stack[next + 1]] =*ptr;	 
    record.memory[addrInRs/0x25000] = 1;
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 3;      
}

void fload(int&next){// load rd rs 
    int addrInRs;
    addrInRs=simStorage.registers[instructionStack->stack[next + 2]]; // address in rs
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    float*ptr=(float*)((void*)(simStorage.memory+addrInRs));
    simStorage.registers[instructionStack->stack[next + 1]] =*ptr;	 
    record.memory[addrInRs/0x25000] = 1;
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 3;      
}

void store(int&next){
    uint addrInRd;
    addrInRd =simStorage.registers[instructionStack->stack[next + 2]]; // address in rd
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    uint*ptr=(uint*)((void*)(simStorage.memory+addrInRd));
    *ptr=simStorage.registers[instructionStack->stack[next + 1]];
    record.reg_read[instructionStack->stack[next + 1]] = 1;
    record.memory[addrInRd/0x25000] = 1;
    next += 3;
}

void fstore(int&next){
    uint addrInRd;
    addrInRd =simStorage.registers[instructionStack->stack[next + 2]]; // address in rd
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    float*ptr=(float*)((void*)(simStorage.memory+addrInRd));
    *ptr=simStorage.registers[instructionStack->stack[next + 1]];
    record.reg_read[instructionStack->stack[next + 1]] = 1;
    record.memory[addrInRd/0x25000] = 1;
    next += 3;
}

void push(int&next){
    uint*ptr=(uint *)(void *)(simStorage.stack + simStorage.stackTop - 3);
    *ptr=simStorage.registers[instructionStack->stack[next + 1]];
    record.reg_read[instructionStack->stack[next + 1]] = 1;
    record.stack = 1;
    simStorage.stackTop -= 4;
    next += 2;
}

void pop(int&next){
    uint *ptr=(uint *)(void *)(simStorage.stack + simStorage.stackTop + 1);
    simStorage.registers[instructionStack->stack[next + 1]] = *ptr;
    record.stack = 1;
    record.reg_read[instructionStack->stack[next + 1]] = 1;
    simStorage.stackTop += 4;
    next += 2;      
}

void mov1(int&next){
    simStorage.registers[instructionStack->stack[next + 1]] =
          simStorage.registers[instructionStack->stack[next + 2]];
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 3;
}

void fmov1(int&next){
    float dest=*(float*)((void*)(simStorage.registers+instructionStack->stack[next+2]));
    *(float*)((void*)(simStorage.registers+instructionStack->stack[next+1]));
    record.reg_read[instructionStack->stack[next + 2]] = 1;
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 3;
}

void mov2(int&next){
    simStorage.registers[instructionStack->stack[next + 1]] =
	*(uint *)(void *)(instructionStack->stack + next + 2);
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 6;    
}

void fmov2(int&next){
    float dest=*(float*)(void *)(instructionStack->stack + next + 2);
    *(float*)((void*)(simStorage.registers+instructionStack->stack[next + 1]))=dest;
    record.reg_write[instructionStack->stack[next + 1]] = 1;
    next += 6;    
}

void jal(int&next){
    next = *(uint *)(void *)(instructionStack->stack + next + 1);
}

void beq(int&next){
    if (simStorage.registers[instructionStack->stack[next + 1]] ==
	simStorage.registers[instructionStack->stack[next + 2]]) 
        next = *(uint *)(void *)(instructionStack->stack + next + 3);
    else
	next+=7;
}

void bne(int&next){
    if (simStorage.registers[instructionStack->stack[next + 1]] !=
	simStorage.registers[instructionStack->stack[next + 2]]) 
        next = *(uint *)(void *)(instructionStack->stack + next + 3);
    else
        next+=7;
}

void blt(int&next){
    if (simStorage.registers[instructionStack->stack[next + 1]] <
	simStorage.registers[instructionStack->stack[next + 2]]) 
        next = *(uint *)(void *)(instructionStack->stack + next + 3);
    else
        next+=7;
}

void bge(int&next){
    if (simStorage.registers[instructionStack->stack[next + 1]] >=
	simStorage.registers[instructionStack->stack[next + 2]]) 
        next = *(uint *)(void *)(instructionStack->stack + next + 3);
    else
        next+=7;
}

void call(int&next){    
      uint* ptr =
          (unsigned int *)(void *)(simStorage.stack + simStorage.stackTop - 3);
      *ptr = next + 5;
      next = *((int *)(void *)(&(instructionStack->stack[next + 1])));      
      record.stack = 1;
      simStorage.stackTop -= 4;
}

void ret(int&next){
    next = *(uint *)(void *)(simStorage.stack + (simStorage.stackTop + 1));
    simStorage.stackTop += 4;
    record.stack = 1;
}

void draw(int&next){
      drawBitmapImage(imageFileNum++);
//      std::cerr<<1.*clock()/CLOCKS_PER_SEC<<std::endl;
      next += 1;
}

void end(int&next){
    outputTxt();
    exit(0);
}

static void init(){
    funcPointer[0x10]=load;
    funcPointer[0x11]=store;
    funcPointer[0x12]=fload;
    funcPointer[0x13]=fstore;
    funcPointer[0x20]=push;
    funcPointer[0x21]=pop;
    funcPointer[0x30]=mov1;
    funcPointer[0x31]=mov2;
    funcPointer[0x32]=fmov1;
    funcPointer[0x33]=fmov2;
    funcPointer[0x40]=add;
    funcPointer[0x41]=add;
    funcPointer[0x42]=sub;
    funcPointer[0x43]=sub;
    funcPointer[0x44]=fadd;
    funcPointer[0x45]=fadd;
    funcPointer[0x46]=fsub;
    funcPointer[0x47]=fsub;
    funcPointer[0x50]=mul;
    funcPointer[0x51]=mul;
    funcPointer[0x52]=div;
    funcPointer[0x53]=div;
    funcPointer[0x54]=rem;
    funcPointer[0x55]=rem;
    funcPointer[0x56]=fmul;
    funcPointer[0x57]=fmul;
    funcPointer[0x58]=fdiv;
    funcPointer[0x59]=fdiv;
    funcPointer[0x60]=and_;
    funcPointer[0x61]=and_;
    funcPointer[0x62]=or_;
    funcPointer[0x63]=or_;
    funcPointer[0x70]=jal;
    funcPointer[0x80]=beq;
    funcPointer[0x81]=bne;
    funcPointer[0x82]=blt;
    funcPointer[0x83]=bge;
    funcPointer[0x90]=call;
    funcPointer[0x91]=ret;
    funcPointer[0xa0]=draw;
    funcPointer[0x00]=end;
}

void MemoryInstruction() {
    init();
    int next = instructionStack->stack[instructionStack->stackTop];
    while (bln) {
//	std::cerr<<"Next="<<std::hex<<next<<std::endl;
//      monitor(std::cerr);
//      monitorMemory(std::cerr);
//	monitor();
	funcPointer[instructionStack->stack[next]](next);
    }
}
