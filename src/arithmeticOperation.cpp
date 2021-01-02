#include<cstdio>
#include<cstring>
#include<tuple>
#include"MemoryInstruction.h"
#include"arithmeticOperation.h"
#include"drawBitmapImage.h"

//simStorage register[] memory[]
//instructionStack->stack[] 

static Memory_4 imm2Memory(int pos){
    Memory_4 tmp;
    for(int i=0;i<4;i++)
	tmp.chr[i]=instructionStack->stack[pos+i];
    return tmp;
}

// pos     ,cur,
//[operator,rs1,1,2,3,4]
//[operator,rs1,rs2]

static std::tuple<Memory_4,Memory_4,int*> prepare(int&pos){
    Memory_4 a,b;
    unsigned char oper=instructionStack->stack[pos];
    int x,y,z;
    int*c=simStorage.registers+(z=instructionStack->stack[++pos]);
    a.val=simStorage.registers[x=instructionStack->stack[++pos]];
    record.reg_read[x]=1;
    record.reg_write[z]=1;
    if(oper&1)
	b=imm2Memory(pos+1),pos+=5;
    else 
	b.val=simStorage.registers[y=instructionStack->stack[++pos]],record.reg_read[y]=1,pos++;
    return std::make_tuple(a,b,c);
}

static std::tuple<Memory_4,Memory_4,float*> fprepare(int&pos){
    Memory_4 a,b;
    unsigned char oper=instructionStack->stack[pos];
    int x,y,z;
    float*c=(float*)((void*)(simStorage.registers+(z=instructionStack->stack[++pos])));
    a.val=simStorage.registers[x=instructionStack->stack[++pos]];
    record.reg_read[x]=1;
    record.reg_write[z]=1;
    if(oper&1)
	b=imm2Memory(pos+1),pos+=5;
    else 
	b.val=simStorage.registers[y=instructionStack->stack[++pos]],record.reg_read[y]=1,pos++;
    return std::make_tuple(a,b,c);
}

void add(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val+b.val;
}

void sub(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val-b.val;
}

void mul(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val*b.val;
}

void div(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val/b.val;
}

void rem(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val%b.val;
}

void and_(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val&b.val;
}

void or_(int&pos){
    Memory_4 a,b;
    int*c;
    std::tie(a,b,c)=prepare(pos);
    *c=a.val|b.val;
}

void fadd(int&pos){
    Memory_4 a,b;
    float*c;
    std::tie(a,b,c)=fprepare(pos);
    *c=a.valf+b.valf;
}

void fsub(int&pos){
    Memory_4 a,b;
    float*c;
    std::tie(a,b,c)=fprepare(pos);
    *c=a.valf-b.valf;
}

void fmul(int&pos){
    Memory_4 a,b;
    float*c;
    std::tie(a,b,c)=fprepare(pos);
    *c=a.valf*b.valf;
}

void fdiv(int&pos){
    Memory_4 a,b;
    float*c;
    std::tie(a,b,c)=fprepare(pos);
    *c=a.valf/b.valf;
}
