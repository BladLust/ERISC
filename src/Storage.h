//
// Created by Timothy.Tsui on 2020/11/27.
//

#ifndef ERISC_STORAGE_H
#define ERISC_STORAGE_H
/**
 * The main structure that simulates computer memory, VERY LARGE, DO NOT COPY BY
 * VALUE.
 */
typedef struct {
  unsigned char stack[0x400010];      // 4 megabytes of memory, with 16 bytes spare;
  unsigned int stackTop = 0; // stack[stackTop] is the location of the NEXT byte
                             // to the last byte of the stack
  unsigned char memory[0x400010];
  int registers[32]={0};
} SimulatorStorageType;
/**
 * struct to hold all instructions.
 */
typedef struct {
  unsigned char stack[0x400010];
  unsigned int stackTop = 0;
} InstructionStackType;
extern SimulatorStorageType simStorage;
#endif // ERISC_STORAGE_H
