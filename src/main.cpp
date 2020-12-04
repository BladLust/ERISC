//
// Created by Timothy.Tsui on 2020/11/27.
//
#include "GeneralFunctions.h"
#include "InstructionParser.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  string input="../input.risc";
  parseInstructions(input);
  dumpBinary(instructionStack->stack, instructionStack->stackTop, 16);
//  ifstream ifs;
//  ifs.open(input);
//  cout<<!ifs<<endl;
//  ifs>>input;
//  cout<<input;
  return 0;
}