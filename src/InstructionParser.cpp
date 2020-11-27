//
// Created by Timothy.Tsui on 2020/11/27.
//

#include "InstructionParser.h"
#include <iostream>
#include <string>
#include <map>
char instructionStack[0x400010];//4 megabytes of memory, with 16 bytes spare

int parseInstructions(const char* dir){
  freopen(dir,'r',stdin);
  string currentInstruction;
  while(cin>>currentInstruction){
    switch () {

    }
  }
  fclose(stdin);
}