//
// Created by Timothy.Tsui on 2020/11/27.
//
#include "GeneralFunctions.h"
#include "InstructionParser.h"
#include "MemoryInstruction.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main() {
  std::string input;
  std::cin>>input;
  int next = parseInstructions(input);
//  dumpBinary(instructionStack->stack, instructionStack->stackTop, 16);  std::cerr<<std::endl;
//  ifstream ifs;
//  ifs.open(input);
//  cerr<<!ifs<<endl;
//  ifs>>input;
//  cout<<input;

  MemoryInstruction(next);
  //drawBitmapImage(MemoryInstRecord.imageFileNum);
  return 0;
}
