//
// Created by Timothy.Tsui on 2020/11/27.
//

#include "InstructionParser.h"
#include "GeneralFunctions.h"
#include <fstream>
#include <map>
#include <vector>
using namespace std;
InstructionStackType instStack;
const InstructionStackType *instructionStack;
/**
 * Inserts data into InstStack
 * @param param The generated unsigned int that contains the instructions.
 * Meaningful digits MUST be on the higher end of the variable.
 * @param size of the MEANINGFUL data, in bytes.
 * @param haveImm Whether the data is followed by an imminent number.
 * @param imm the value of the imminent number
 */
static inline void insertIntToInstStack(unsigned int &param, char size,
                                        bool haveImm = false, int imm = 0) {
  int *loc = (int *)(void *)(instStack.stack + instStack.stackTop);
  *loc = param;               // store the data into stac
  instStack.stackTop += size; // increase stacktop, the extra long part of the
                              // data will be later covered
  if (haveImm) { // fill in the imminent number (if there's any) after the main
                 // instruction.
    loc = (int *)(void *)(instStack.stack + instStack.stackTop);
    *loc = imm;
    instStack.stackTop += 4;
  }
  return;
}
/**
 * converts string to int. supports both Dec and Hex
 * @param str the input string
 * @return the extracted number
 */
static unsigned int stringToInt(const string &str) {
  unsigned int result = 0, mult;
  char len = str.length();
  if (str[1] == 'x') { // number is in hex
    mult = 0;
    for (int i = len - 1; i > 1; ++i) {
      result |= (str[i] - '0') << mult;
      mult += 4;
    }
  } else { // number in dec
    mult = 1;
    for (int i = len - 1; i > -1; ++i) {
      result += (str[i] - '0') * mult;
      mult *= 10;
    }
  }
  return result;
}
/**
 * Actual instruction parser, handles register and imminent number input.
 * @param instruction the code for the instruction (e.g.0x60 for and)
 * @param ifs file stream
 * @param totalLen total length of the instruction(the non-imminent number
 * version), in bytes, including the instruction itself.
 * @param canHaveImm whether it's possible for the last parameter of the
 * instruction to be an imminent number
 */
static void parseInstructionNoAddr(char instruction, ifstream &ifs,
                                   char totalLen, bool canHaveImm) {
  unsigned int tempInstruction = 0;
  string params;
  unsigned char bitMove = 0;
  unsigned int imm = 0;
  char haveImm = 0;
  while (--totalLen) {
    ifs >> params;
    if (canHaveImm && totalLen == 1 && params[0] <= '9' && params[0] >= '0') {
      imm = stringToInt(params);
      haveImm = 1;
    } else {
      tempInstruction |= registerNameToLoc(params) << bitMove;
      bitMove += 8;
    }
  }
  tempInstruction = ((unsigned int)instruction + haveImm) << bitMove;
  tempInstruction <<= (4 - totalLen + haveImm) * 8;
  insertIntToInstStack(tempInstruction, totalLen - haveImm, haveImm, imm);
  return;
}

/**
 * Internal helper function, insert new address to the corresponding line mark
 * for it to be later filled. Will reserve the space needed to store the address
 * in instStack.
 * @param ifs file stream
 * @param jumpMap reference to the map
 */
const inline void markJumpAddress(ifstream &ifs,
                                  map<string, vector<unsigned int>> &jumpMap) {
  string tempMark;
  map<string, vector<unsigned int>>::iterator jumpIter;
  ifs >> tempMark;
  jumpIter = jumpMap.find(tempMark);
  if (jumpIter == jumpMap.end()) {
    vector<unsigned int> temp;
    temp.push_back(instStack.stackTop);
    jumpMap.insert({tempMark, temp})
  } else {
    jumpIter->second.push_back(instStack.stackTop);
  }
  instStack.stackTop += 4;
}
int parseInstructions(string dir) {
  instructionStack=&instStack;
  map<string, unsigned int> markMap;
  map<string, vector<unsigned int>> jumpMap;
  ifstream ifs(dir);
  string currentInstruction;
  bool whileBreakFlag = false;
  while (ifs >> currentInstruction) {
    if (currentInstruction[currentInstruction.length() - 1] == ':') {
      currentInstruction.pop_back(); // C++11 feature
      markMap.insert(
          {currentInstruction,
           instStack.stackTop}); // New mark, record the corresponding address in markMap
    } else {
      switch (currentInstruction[0]) { // Start distinguishing each different
                                       // instructions
      case 'l':
        parseInstructionNoAddr((char)0x10, ifs, 3, false); // load
        break;
      case 's':
        if (currentInstruction[1] == 'u') {
          parseInstructionNoAddr((char)0x42, ifs, 4, true); // sub
          continue;
        }
        parseInstructionNoAddr((char)0x11, ifs, 3, false); // store
        break;
      case 'p':
        parseInstructionNoAddr((char)(currentInstruction[1] == 'u') ? 0x20
                                                                    : 0x21,
                               ifs, 2, false); // push and pop
        break;
      case 'm':
        if (currentInstruction[1] == 'o') {
          parseInstructionNoAddr((char)0x30, ifs, 3, true); // mov
          break;
        }
        parseInstructionNoAddr((char)0x50, ifs, 4, true); // mul
        break;
      case 'a':
        parseInstructionNoAddr((char)(currentInstruction[1] == 'n') ? 0x60
                                                                    : 0x40,
                               ifs, 4, true); // add and and
        break;
      case 'd':
        if (currentInstruction[1] == 'r') {
          parseInstructionNoAddr((char)0xa0, ifs, 1, false); // draw
          break;
        }
        parseInstructionNoAddr((char)0x52, ifs, 4, true); // div
        break;
      case 'r':
        if (currentInstruction[2] == 'm') {
          parseInstructionNoAddr((char)0x54, ifs, 4, true); // rem
          break;
        }
        parseInstructionNoAddr((char)0x91, ifs, 1, false); // ret
        break;
      case 'o':
        parseInstructionNoAddr((char)0x62, ifs, 4, true); // or
        break;
      case 'j':
        parseInstructionNoAddr((char)0x70, ifs, 1, false); // jal
        markJumpAddress(ifs, jumpMap);
        break;
      case 'c':
        parseInstructionNoAddr((char)0x90, ifs, 1, false); // call
        markJumpAddress(ifs, jumpMap);
        break;

      case 'b':
        switch (currentInstruction[1]) { // The four conditional jumps
        case 'e':
          parseInstructionNoAddr((char)0x80, ifs, 3, false);
          break;
        case 'n':
          parseInstructionNoAddr((char)0x81, ifs, 3, false);
          break;
        case 'l':
          parseInstructionNoAddr((char)0x82, ifs, 3, false);
          break;
        case 'g':
          parseInstructionNoAddr((char)0x83, ifs, 3, false);
          break;
        }
        markJumpAddress(ifs, jumpMap);
        break;
      case 'e':
        whileBreakFlag = true;
        parseInstructionNoAddr((char)0x00, ifs, 1, false); // end
        break;
      }
    }
    if (whileBreakFlag)
      break;
  }
  ifs.close();
  /* All jump marks should be collected, filling all empty address blocks in
   * instStack that we have recorded in jumpMap*/
  map<string, vector<unsigned int>>::iterator jumpIter;
  for (auto iter = markMap.begin(); iter != markMap.end(); ++iter) {
    jumpIter = jumpMap.find(iter->first);
    while (!jumpIter->second.empty()) {
      *(unsigned int *)(void *)(instStack.stack + jumpIter->second.back()) =
          iter->second;
      jumpIter->second.pop_back();
    }
  }
  return 0;
}
