//
// Created by Timothy.Tsui on 2020/11/27.
//

#ifndef ERISC_GENERALFUNCTIONS_H
#define ERISC_GENERALFUNCTIONS_H
#include <string>
/**
 * Turns the corresponding register name string to their location ,can use like registers[registerNameToLoc("0x")]
 * @param name name of the register
 * @return position of the register in array
 */
unsigned int registerNameToLoc(const std::string &name);
/**
 * dumps everything in the array in Hex, for length len, and lineLen bytes per line.
 * @param arr the pointer to the array
 * @param len total length for output, in Bytes
 * @param lineLen how many bytes per line.
 */
void dumpBinary(const void* arr, int len,int lineLen);

#endif // ERISC_GENERALFUNCTIONS_H
