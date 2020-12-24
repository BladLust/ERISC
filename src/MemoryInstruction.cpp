//
//  main.cpp
//  ERISC
//
//  Created by Wu Weiyan on 11/27/20.
//

#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <map>
#include <vector>
#include "MemoryInstruction.h"

//const InstructionStackType *instructionStack; // maybe want to delete it

void MemoryInstruction(char* instruction){
    InstructionStackType instStack;
    while(std::cin >> instStack != EOF){
        unsigned char takeout = instStack[0];

        switch(takeout){
            case 0x10 ://load
                unsigned char* rd = instStack[2]; //1 byte = 8 bits, 抽出0x00
                unsigned char* rs = instStack[1]; //1 byte
                int rd_idx = int_convert(rd);
                SimulatorStorageType.registers[rd_idx] = *(SimulatorStorageType.memory[rs]);
                break;

            case 0x11 ://store
                unsigned char* rd = instStack[1];
                unsigned char* rs = instStack[1];
                int rs_idx = int_convert(rs);
                SimulatorStorageType.memory[rd] = *(SimulatorStorageType.registers[rs_idx])
                break;
            
            case 0x20 ://push
                unsigned char* rs = instStack[1];
                int rs_idx = int_convert(rs);
                SimulatorStorageType.stack[SimulatorStorageType.stackTop] = *(SimulatorStorageType.registers[rs_idx]);
                SimulatorStorageType.stackTop --;
                break;
                
            case 0x21 ://pop
                unsigned char* rd = instStack[1];
                int rd_idx = int_convert(rd);
                *(SimulatorStorageType.registers[rd_idx]) = stack[SimulatorStorageType.stackTop];
                SimulatorStorageType.stackTop ++;
                return stack[SimulatorStorageType.stackTop - 1];
            
            case 0x30 ://mov_1
                unsigned char* rd = instStack[1]; 
                unsigned char* rs = instStack[2]; 
                int rd_idx = int_convert(rd);
                int rs_idx = int_convert(rs);
                *(SimulatorStorageType.registers[rd_idx]) = *(SimulatorStorageType.registers[rs_idx];
                break;
 
            case 0x31 ://mov_2
                unsigned char* rd = instStack[1]; 
                int* imm = instStack[2];
                *(SimulatorStorageType.registers[rd_idx]) = imm;
                break;
            
            case 0x70 ://jal
                next = *((int*)instStack[next+1]);
                break;

            case 0x80 ://beq
                unsigned char* rs1 = instStack[1]; 
                unsigned char* rs2 = instStack[2]; 
                int rs1_idx = int_convert(rs1);
                int rs2_idx = int_convert(rs2);
                if (*(SimulatorStorageType.registers[rs1_idx]) == *(SimulatorStorageType.registers[rs2_idx])){
                    next = *((int*)instStack[next+1]);
                }
                break;

            case 0x81 ://bne
                unsigned char* rs1 = instStack[1]; 
                unsigned char* rs2 = instStack[2]; 
                int rs1_idx = int_convert(rs1);
                int rs2_idx = int_convert(rs2);
                if (*(SimulatorStorageType.registers[rs1_idx]) != *(SimulatorStorageType.registers[rs2_idx])){
                    next = *((int*)instStack[next+1]);
                }
                break;
            
            case 0x82 ://blt
                unsigned char* rs1 = instStack[1]; 
                unsigned char* rs2 = instStack[2]; 
                int rs1_idx = int_convert(rs1);
                int rs2_idx = int_convert(rs2);
                if (*(SimulatorStorageType.registers[rs1_idx]) < *(SimulatorStorageType.registers[rs2_idx])){
                    next = *((int*)instStack[next+1]);
                }
                break;

            
            case 0x83 ://bge
                unsigned char* rs1 = instStack[1]; 
                unsigned char* rs2 = instStack[2]; 
                int rs1_idx = int_convert(rs1);
                int rs2_idx = int_convert(rs2);
                if (*(SimulatorStorageType.registers[rs1_idx]) >= *(SimulatorStorageType.registers[rs2_idx])){
                    next = *((int*)instStack[next+1]);
                }
                break;
        }
               
    }
    
}

int int_convert(unsigned char * rds){
    if (*rds == 0x00){
        return 0;
    }
    else if (*rds == 0x01){
        return 1;
    }
    else if (*rds == 0x02){
        return 2;
    }
    else if (*rds == 0x03){
        return 3;
    }
    else if (*rds == 0x04){
        return 4;
    }
    else if (*rds == 0x05){
        return 5;
    }
    else if (*rds == 0x06){
        return 6;
    }
    else if (*rds == 0x07){
        return 7;
    }
    else if (*rds == 0x08){
        return 8;
    }
    else if (*rds == 0x09){
        return 9;
    }
    else if (*rds == 0x10){
        return 10;
    }
    else if (*rds == 0x11){
        return 11;
    }
    else if (*rds == 0x12){
        return 12;
    }
    else if (*rds == 0x13){
        return 13;
    }
    else if (*rds == 0x14){
        return 14;
    }
    else if (*rds == 0x15){
        return 15;
    }
    else if (*rds == 0x16){
        return 16;
    }
    else if (*rds == 0x17){
        return 17;
    }
    else if (*rds == 0x18){
        return 18;
    }
    else if (*rds == 0x19){
        return 19;
    }
    else if (*rds == 0x20){
        return 20;
    }
    else if (*rds == 0x21){
        return 21;
    }
    else if (*rds == 0x22){
        return 22;
    }
    else if (*rds == 0x23){
        return 23;
    }
    else if (*rds == 0x24){
        return 24;
    }
    else if (*rds == 0x25){
        return 25;
    }
    else if (*rds == 0x26){
        return 26;
    }
    else if (*rds == 0x27){
        return 27;
    }
    else if (*rds == 0x28){
        return 28;
    }
    else if (*rds == 0x29){
        return 29;
    }
    else if (*rds == 0x30){
        return 30;
    }
    else if (*rds == 0x31){
        return 31;
    }

}