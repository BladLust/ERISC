//
// Created by Timothy.Tsui on 2020/11/27.
//
#include "InstructionParser.h"
#include "GeneralFunctions.h"
#include "Settings.h"
#include <fstream>
#include <map>
#include <vector>

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
  unsigned int *loc =
      (unsigned int *)(void *)(instStack.stack + instStack.stackTop);
  *loc = param;               // store the data into stac
  instStack.stackTop += size; // increase stacktop, the extra long part of the
                              // data will be later covered
  if (haveImm) { // fill in the imminent number (if there's any) after the main
                 // instruction.
    loc = (unsigned int *)(void *)(instStack.stack + instStack.stackTop);
    *loc = imm;
    instStack.stackTop += 4;
  }
  //  dumpBinary(instructionStack->stack, instructionStack->stackTop, 16);
  return;
}
/**
 * converts string to int. supports both Dec and Hex
 * @param str the input string
 * @return the extracted number
 */
static unsigned int stringToInt(const std::string &str) {
  unsigned int result = 0, mult;
  char len = str.length();
  if (len > 1 && str[1] == 'x') { // number is in hex
    mult = 0;
    for (int i = len - 1; i > 1; --i) {
      if (str[i] > '9')
        result |= (unsigned int)(str[i] - 'a' + 10) << mult;
      else
        result |= (unsigned int)(str[i] - '0') << mult;
      mult += 4;
    }
  } else { // number in dec
    mult = 1;
    for (int i = len - 1; i > -1; --i) {

      result += (unsigned int)(str[i] - '0') * mult;
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
static void parseInstructionNoAddr(unsigned char instruction,
                                   std::ifstream &ifs, char totalLen,
                                   bool canHaveImm) {
#if DEBUG_LVL >= 5
  std::cout << "Debug:" << (int *)instruction << ' ' << totalLen << std::endl;
#endif
  unsigned int tempInstruction = 0;
  char params[15];
  unsigned char bitMove = 8;
  unsigned int imm = 0;
  char haveImm = 0;
  char len = totalLen;
  while (--len) {
    ifs.get(params, 2);
    if (len == 1)
      if ((instruction & 0xF0) == 0x80) {
        ifs.get(params, 11, ',');
        ifs.get(params + 13, 2);
      } else
        ifs >> params;
    else
      ifs.get(params, 11, ',');
    if (canHaveImm && len == 1 && params[0] <= '9' && params[0] >= '0') {
      imm = stringToInt(params);
      haveImm = 1;
    } else {
      tempInstruction |= registerNameToLoc(params) << bitMove;
      bitMove += 8;
    }
  }
  tempInstruction |= ((unsigned int)instruction + haveImm);
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
const inline void
markJumpAddress(std::ifstream &ifs,
                std::map<std::string, std::vector<unsigned int> > &jumpMap) {
  std::string tempMark;
  std::map<std::string, std::vector<unsigned int> >::iterator jumpIter;
  ifs >> tempMark;
#if DEBUG_LVL >= 5
  std::cout << "Debug: tempMark" << tempMark << std::endl;
#endif
  jumpIter = jumpMap.find(tempMark);
  if (jumpIter == jumpMap.end()) {
    std::vector<unsigned int> temp;
    temp.push_back(instStack.stackTop);
    jumpMap[tempMark] = temp;
  } else {
    jumpIter->second.push_back(instStack.stackTop);
  }
  instStack.stackTop += 4;
}
int parseInstructions(std::string dir) {
#if DEBUG_LVL >= 5
  std::cout << "Debug: " << dir << std::endl;
#endif
  instructionStack = &instStack;
  std::map<std::string, unsigned int> markMap;
  std::map<std::string, std::vector<unsigned int> > jumpMap;
  std::ifstream ifs(dir);
  std::string currentInstruction;
  while (ifs >> currentInstruction) {
    if (currentInstruction[currentInstruction.length() - 1] == ':') {
      currentInstruction.pop_back(); // C++11 feature
      markMap[currentInstruction] =
          instStack.stackTop; // New mark, record the corresponding address in
                              // markMap
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
          parseInstructionNoAddr((unsigned char)0xa0, ifs, 1, false); // draw
          break;
        }
        parseInstructionNoAddr((char)0x52, ifs, 4, true); // div
        break;
      case 'r':
        if (currentInstruction[2] == 'm') {
          parseInstructionNoAddr((char)0x54, ifs, 4, true); // rem
          break;
        }
        parseInstructionNoAddr((unsigned char)0x91, ifs, 1, false); // ret
        break;
      case 'o':
        parseInstructionNoAddr((char)0x62, ifs, 4, true); // or
        break;
      case 'j':
        parseInstructionNoAddr((char)0x70, ifs, 1, false); // jal
        markJumpAddress(ifs, jumpMap);
        break;
      case 'c':
        parseInstructionNoAddr((unsigned char)0x90, ifs, 1, false); // call
        markJumpAddress(ifs, jumpMap);
        break;

      case 'b':
        switch (currentInstruction[1]) { // The four conditional jumps
        case 'e':
          parseInstructionNoAddr((unsigned char)0x80, ifs, 3, false);
          break;
        case 'n':
          parseInstructionNoAddr((unsigned char)0x81, ifs, 3, false);
          break;
        case 'l':
          parseInstructionNoAddr((unsigned char)0x82, ifs, 3, false);
          break;
        case 'g':
          parseInstructionNoAddr((unsigned char)0x83, ifs, 3, false);
          break;
        }
        markJumpAddress(ifs, jumpMap);
        break;
      case 'e':
        parseInstructionNoAddr((char)0x00, ifs, 1, false); // end
        break;
      }
    }
  }
  ifs.close();
  /* All jump marks should be collected, filling all empty address blocks in
   * instStack that we have recorded in jumpMap*/
  std::map<std::string, std::vector<unsigned int> >::iterator jumpIter;
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
