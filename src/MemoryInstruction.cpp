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
#include "drawBitmapImage.h"

void MemoryInstruction(int next)
{
  next = instructionStack->stack[instructionStack->stackTop];
  while (true)
  {
    switch (instructionStack->stack[next])
    {
    case 0x10: //load
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.memory[instructionStack->stack[next + 2]];
      //record->reg_write[instructionStack->stack[next+1]] = 1;
      //record->memory[instructionStack->stack[next+2]] = 1;
      next += 3;
      break;

    case 0x11: //store
      simStorage.memory[instructionStack->stack[next + 1]] = simStorage.registers[instructionStack->stack[next + 2]];
      //record->reg_read[instructionStack->stack[next+2]] = 1;
      next += 3;
      break;

    case 0x20: //push
      simStorage.stack[instructionStack->stackTop] = simStorage.registers[instructionStack->stack[next + 1]];
      //record->reg_read[instructionStack->stack[next+1]] = 1;
      //record->stack = 1;
      simStorage.stackTop--;
      break;

    case 0x21: //pop
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.stack[instructionStack->stackTop];
      simStorage.stackTop++;
      //record->stack = 1;
      simStorage.stack[instructionStack->stackTop - 1];
      break;

    case 0x30: //mov_1
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.registers[instructionStack->stack[next + 2]];
      //record->reg_write[instructionStack->stack[next+1]] = 1;
      //record->reg_read[instructionStack->stack[next+2]] = 1;
      next += 3;
      break;

    case 0x31: //mov_2
      simStorage.registers[instructionStack->stack[next + 1]] = simStorage.registers[instructionStack->stack[next + 2]];
      //record->reg_write[instructionStack->stack[next+1]] = 1;
      //record->reg_read[instructionStack->stack[next+2]] = 1;
      next += 6;
      break;

    case 0x70: //jal
      next = simStorage.stack[instructionStack->stack[next + 1]];
      //record->stack = 1;
      break;

    case 0x80: //beq
      if (simStorage.registers[instructionStack->stack[next + 1]] == simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record->reg_read[instructionStack->stack[next+1]] = 1;
        //record->reg_read[instructionStack->stack[next+2]] = 1;
        next = simStorage.stack[instructionStack->stack[next + 1]];
      }
      break;

    case 0x81: //bne
      if (simStorage.registers[instructionStack->stack[next + 1]] != simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record->reg_read[instructionStack->stack[next+1]] = 1;
        //record->reg_read[instructionStack->stack[next+2]] = 1;
        next = simStorage.stack[instructionStack->stack[next + 1]];
      }
      break;

    case 0x82: //blt
      if (simStorage.registers[instructionStack->stack[next + 1]] < simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record->reg_read[instructionStack->stack[next+1]] = 1;
        //record->reg_read[instructionStack->stack[next+2]] = 1;
        next = simStorage.stack[instructionStack->stack[next + 1]];
      }
      break;

    case 0x83: //bge
      if (simStorage.registers[instructionStack->stack[next + 1]] >= simStorage.registers[instructionStack->stack[next + 2]])
      {
        //record->reg_read[instructionStack->stack[next+1]] = 1;
        //record->reg_read[instructionStack->stack[next+2]] = 1;
        next = simStorage.stack[instructionStack->stack[next + 1]];
      }
      break;

    case 0x90:                                                    //call
      next = simStorage.stack[instructionStack->stack[next + 1]]; //我接下来直接把push搬过来了
      simStorage.stack[instructionStack->stackTop] = simStorage.registers[instructionStack->stack[next + 1]];
      //record->reg_read[instructionStack->stack[next+1]] = 1;
      simStorage.stackTop--;
      //record->stack = 1;
      break;

    case 0x91: //ret
      //怎么出栈得出一个行号？return吗
      //record->stack = 1;
      break;

    case 0xa0: //draw
      MemoryInstRecord.imageFileNum++;
      break;

    case 0x00: //end
      break;

    default : //Optional
      arithmeticOp(next, instructionStack->stack[next]);
    }
  }
}