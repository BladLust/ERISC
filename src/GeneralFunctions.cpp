//
// Created by Timothy.Tsui on 2020/11/27.
//
#include "GeneralFunctions.h"
#include <iomanip>
#include <iostream>

static std::string regNames[9] = {"zero", "ra", "sp", "gp", "tp",
                                  "t0",   "t1", "t2", "fp"};
unsigned int registerNameToLoc(const std::string &name) {
  char tempIndex;
  switch (name[0]) {
  case 'x':
    return (name.length() > 2) ? ((name[1] - '0') * 10 + name[2] - '0')
                               : name[1] - '0';
  case 'a':
    return name[1] - '0' + 10;
  case 't':
    tempIndex = name[1] - '0';
    return (tempIndex < 3) ? tempIndex + 5 : tempIndex + 25;
  case 's':
    tempIndex = name[1] - '0';
    if (name.length() > 2) {
      return (name[1] - '0') * 10 + name[2] - '0' + 16;
    } else
      return (tempIndex < 2) ? tempIndex + 8 : tempIndex + 16;
  default:
    for (unsigned char i = 0; i < 9; ++i) {
      if (name.compare(regNames[i]))
        return i;
    }
  }
}
void dumpBinary(const void *arr, int len, int lineLen) {
  int count = 0;
  unsigned char *dumpArr = (unsigned char *)arr;
  std::cout.fill('0');
  while (len--) {
    if (count % lineLen == 0) {
      std::cout << "0x" << std::hex << std::setw(8) << count << ' ';
    }
    std::cout << std::hex << std::setw(2) << (int)dumpArr[count] << ' ';
    if (count % lineLen == lineLen - 1) {
      std::cout << std::endl;
    }
    count++;
  }
}
