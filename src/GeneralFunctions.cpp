//
// Created by Timothy.Tsui on 2020/11/27.
//

#include "GeneralFunctions.h"
static std::string regNames[9]={"zero","ra","sp","gp","tp","t0","t1","t2","fp"};
unsigned int registerNameToLoc(const std::string& name){
   char tempIndex;
  switch(name[0]){
  case 'x':
    return name[1]-'0';
  case'a':
    return name[1]-'0'+10;
  case't':
    tempIndex=name[1]-'0';
    return (tempIndex<3)?tempIndex+5:tempIndex+25;
  case's':
    tempIndex=name[1]-'0';
    return (tempIndex<2)?tempIndex+8:tempIndex+16;
  default:
    for (unsigned char i = 0; i < 9; ++i) {
      if(name.compare(regNames[i]))
        return i;
    }
  }
}
