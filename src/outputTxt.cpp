//
// Created by Ying Yuan on 2020/12/10
//

#include "GeneralFunctions.h"
#include "Storage.h"
#include <iostream>
#include <fstream>
using namespace std;

void outputTxt(){
    ofstream write;
    write.open("result.txt");
    for (int i = 0; i <= 30; i++) {
        write << hex << (unsigned int)simStorage.registers[i] << " ";
    }
    write << hex << (unsigned int)simStorage.registers[31] << endl;
    int times = 63;
    for (int i = 0x000000; i <= 0x3fffff; i++) {
        if (times > 0) {
            write << hex << (unsigned int)simStorage.memory[i] << " ";
            times--;
        }
        else {
            write << hex << (unsigned int)simStorage.memory[i] << endl;
            times = 63;
        } 
    }
    write.close();
}