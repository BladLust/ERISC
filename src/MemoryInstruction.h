//
// Created by Timothy.Tsui on 2020/12/25.
//

#ifndef ERISC_MEMORYINSTRUCTION_H
#define ERISC_MEMORYINSTRUCTION_H
#include "InstructionParser.h"
#include "GeneralFunctions.h"
#include "Settings.h"
#include "Storage.h"
//#include "outputTxt.h"

void MemoryInstruction();

typedef void (*Function)(int&);
//extern Function funcPointer[0xff];

#endif // ERISC_MEMORYINSTRUCTION_H
