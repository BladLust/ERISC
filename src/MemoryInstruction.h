//
// Created by Timothy.Tsui on 2020/12/25.
//

#ifndef ERISC_MEMORYINSTRUCTION_H
#define ERISC_MEMORYINSTRUCTION_H
#include "InstructionParser.h"
#include "GeneralFunctions.h"
#include "Settings.h"
#include "Storage.h"
#include "drawBitmapImage.h"
//void MemoryInstruction(char* instruction);
typedef struct {
  int imageFileNum = 0;
} MemoryInst;
void MemoryInstruction(int next);
extern MemoryInst MemoryInstRecord;

#endif // ERISC_MEMORYINSTRUCTION_H
