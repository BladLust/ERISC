//
// Created by Timothy.Tsui on 2020/11/27.
//

#ifndef ERISC_INSTRUCTIONPARSER_H
#define ERISC_INSTRUCTIONPARSER_H

#include <string>
#include "Storage.h"
/**
 * Reads the corresponding file and compile it into binary
 * @param name directory to the file, can be either relative or absolute
 * @return 0 if successful, but if it fails it would also return 0 :P
 */
int parseInstructions(std::string dir);

/**
 * Pointer to the instruction Stack
 * WARNING: USE AFTER CALLING 'parseInstructions', OR THIS POINTER IS NULLPTR
 */
const InstructionStackType *instructionStack;
#endif // ERISC_INSTRUCTIONPARSER_H
