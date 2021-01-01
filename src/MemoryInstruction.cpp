//
//  main.cpp
//  ERISC
//
//  Created by Wu Weiyan on 11/27/20.
//

#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <map>
#include <vector>
#include "MemoryInstruction.h"
#include "InstructionParser.h"
#include "GeneralFunctions.h"

bool bln = true;
unsigned char a0, a1, a2, a3;
int imageFileNum;

//function which converts four unsigned char to an integer
int convertToInt(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3)
{
    unsigned char a[4];
    a[0] = a0;
    a[1] = a1;
    a[2] = a2;
    a[3] = a3;
    int x;
    x = (a[0]<<24) | (a[1]<<16) | (a[2]<<8) | a[3];
    return x;
}

void MemoryInstruction(int next)
{
    uint*ptr;
  next = instructionStack->stack[instructionStack->stackTop];
  while (bln)
  {
      std::cerr<<"Next="<<std::hex<<next<<std::endl;
    switch (instructionStack->stack[next])
    {

    case 0x10: //load
      int addrInRs;
      addrInRs = simStorage.registers[instructionStack->stack[next + 2]]; // address in rs
      //record.reg_read[instructionStack->stack[next + 2]] = 1;
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.memory[addrInRs];
      //record.memory[addrInRs] = 1;
      //record.reg_write[instructionStack->stack[next + 1]] = 1;
      next += 3;
      break;

    case 0x11: //store
      int addrInRd;
      addrInRd = simStorage.registers[instructionStack->stack[next + 2]]; //address in rd
      //record.reg_read[instructionStack->stack[next + 2]] = 1;
      simStorage.memory[addrInRd] = simStorage.registers[instructionStack->stack[next + 1]];
      //record.reg_read[instructionStack->stack[next + 1]] = 1;
      //record.memory[addrInRd] = 1;
      next += 3;
      std::cerr<<"Next|="<<std::hex<<next<<std::endl;
      break;

    case 0x20: //push
      simStorage.stack[instructionStack->stackTop] = simStorage.registers[instructionStack->stack[next + 1]];
      //record.reg_read[instructionStack->stack[next + 1]] = 1;
      //record.stack = 1;
      simStorage.stackTop--;
      break;

    case 0x21: //pop
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.stack[instructionStack->stackTop];
      //record.stack = 1;
      //record.reg_read[instructionStack->stack[next + 1]] = 1;
      simStorage.stackTop++;
      break;

    case 0x30: //mov_1
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.registers[instructionStack->stack[next + 2]];
      //record.reg_read[instructionStack->stack[next + 2]] = 1;
      //record.reg_write[instructionStack->stack[next + 1]] = 1;
      next += 3;
      //bln = false;//test only
      break;

    case 0x31: //mov_2
      a0 = instructionStack->stack[next + 4];
      a1 = instructionStack->stack[next + 3];
      a2 = instructionStack->stack[next + 2];
      a3 = instructionStack->stack[next + 1];
      int imm;
      imm = convertToInt(a0, a1, a2, a3);
      simStorage.registers[instructionStack->stack[next + 1]] = imm;
      //record.reg_write[instructionStack->stack[next + 1]] = 1;
      //std:: cout << "reg is " << imm; //test only
      next += 6;
      //bln = false;//test only
      break;

    case 0x70: //jal
      a0 = instructionStack->stack[next + 4];
      a1 = instructionStack->stack[next + 3];
      a2 = instructionStack->stack[next + 2];
      a3 = instructionStack->stack[next + 1];
      //std:: cout << "next is " << next;//test only
      //std::cout << "  a0 is "<< a0 << " finish ";//test only
      int index;
      index = convertToInt(a0, a1, a2, a3);
      next = index;
      //std:: cout << "next is " << next;//test only
      //bln = false;//test only
      break;

    case 0x80: //beq
      if (simStorage.registers[instructionStack->stack[next + 1]] == simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record.reg_read[instructionStack->stack[next + 1]] = 1;
        //record.reg_read[instructionStack->stack[next + 2]] = 1;
        a0 = instructionStack->stack[next + 6];
        a1 = instructionStack->stack[next + 5];
        a2 = instructionStack->stack[next + 4];
        a3 = instructionStack->stack[next + 3];
        int index;
        index = convertToInt(a0, a1, a2, a3);
        next = index;
      }
      break;

    case 0x81: //bne
      if (simStorage.registers[instructionStack->stack[next + 1]] != simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record.reg_read[instructionStack->stack[next + 1]] = 1;
        //record.reg_read[instructionStack->stack[next + 2]] = 1;
        a0 = instructionStack->stack[next + 6];
        a1 = instructionStack->stack[next + 5];
        a2 = instructionStack->stack[next + 4];
        a3 = instructionStack->stack[next + 3];
        int index;
        index = convertToInt(a0, a1, a2, a3);
        next = index;
      }
      break;

    case 0x82: //blt
      if (simStorage.registers[instructionStack->stack[next + 1]] < simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record.reg_read[instructionStack->stack[next + 1]] = 1;
        //record.reg_read[instructionStack->stack[next + 2]] = 1;
        a0 = instructionStack->stack[next + 6];
        a1 = instructionStack->stack[next + 5];
        a2 = instructionStack->stack[next + 4];
        a3 = instructionStack->stack[next + 3];
        int index;
        index = convertToInt(a0, a1, a2, a3);
        next = index;
      }
      break;

    case 0x83: //bge
      if (simStorage.registers[instructionStack->stack[next + 1]] >= simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record.reg_read[instructionStack->stack[next + 1]] = 1;
        //record.reg_read[instructionStack->stack[next + 2]] = 1;
        a0 = instructionStack->stack[next + 6];
        a1 = instructionStack->stack[next + 5];
        a2 = instructionStack->stack[next + 4];
        a3 = instructionStack->stack[next + 3];
        int index;
        index = convertToInt(a0, a1, a2, a3);
        next = index;
      }
      break;

    case 0x90: //call
//	next = (void*)instructionStack->stack[instructionStack->stack[next + 1]];
      //record.reg_read[instructionStack->stack[next + 1]] = 1;
        ptr=(unsigned int*)(void*)(simStorage.stack+simStorage.stackTop-3);
	fprintf(stderr,"%p %p\n",(uint*)(void*)(simStorage.stack+simStorage.stackTop-3),simStorage.stack+0x400000);
	*ptr=next+5;
	std::cerr<<std::hex<<"next="<<next<<' '<<"*ptr="<<*ptr<<std::endl;
	next=*((int*)(void*)(&(instructionStack->stack[next+1])));
	dumpBinary(simStorage.stack+simStorage.stackTop-3,4,4);
//	simStorage.stack[instructionStack->stackTop] = (uint)(next+1)//simStorage.registers[instructionStack->stack[next + 1]];
      //record.stack = 1;
      simStorage.stackTop-=4;
      break;

    case 0x91: //ret
	next =*(uint*)(void*)(simStorage.stack+(simStorage.stackTop+1));
      simStorage.stackTop+=4;
      //record.stack = 1;
      break;

    case 0xa0: //draw
      //imageFileNum++;
      //drawBitmapImage(imageFileNum);
      next += 1;
      break;

    case 0x00: //end
      //outputTxt();
	exit(0);
      break;

    default : //Optional
      //arithmeticOp(next);
      break;
    }
  }
}
