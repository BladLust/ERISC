#include<cstdio>
#include<cstring>
#include"MemoryInstruction.h"
#include"arithmeticOperation.h"
#include "drawBitmapImage.h"

// each char contains two hex bytes, e.g, 8 bits;
// a int contains 32 bits;

// little_endian
// if val=0x12345678
// then chr={78,56,34,12};

// lhs[4]={a_0,a_1,a_2,a_3}

static Memory_4 Add(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val+rhs.val;
    return res;
}

static Memory_4 Sub(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val-rhs.val;
    return res;
}

static Memory_4 Mul(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val*rhs.val;
    return res;
}

static Memory_4 Div(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val/rhs.val;
    return res;
}

static Memory_4 Rem(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val%rhs.val;
    return res;
}

static Memory_4 And(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val&rhs.val;
    return res;
}

static Memory_4 Or(Memory_4 lhs,Memory_4 rhs){
    Memory_4 res;
    res.val=lhs.val|rhs.val;
//    std::cerr<<std::hex<<"Or: "<<lhs.val<<' '<<rhs.val<<std::endl;
    return res;
}

static void Debug(Memory_4 x){
    fprintf(stderr,"%8d=",x.val);
    for(int i=0;i<4;i++)
        fprintf(stderr,"%02x ",x.chr[i]);
    fprintf(stderr,"\n");
}

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

void arithmeticOp(int&pos){
    Memory_4 a,b;
    unsigned char oper=instructionStack->stack[pos];
    int x,y,z;
    int&c=simStorage.registers[z=instructionStack->stack[++pos]];
    a.val=simStorage.registers[x=instructionStack->stack[++pos]];
    record.reg_read[x]=1;
    record.reg_write[z]=1;
    if(oper&1)
	b=imm2Memory(pos+1),pos+=5;
    else 
	b.val=simStorage.registers[y=instructionStack->stack[++pos]],record.reg_read[y]=1,pos++;
    switch(oper){	
    case 0x40:case 0x41:
	c=Add(a,b).val;break;
    case 0x42:case 0x43:
	c=Sub(a,b).val;break;
    case 0x50:case 0x51:
	c=Mul(a,b).val;break;
    case 0x52:case 0x53:
	c=Div(a,b).val;break;
    case 0x54:case 0x55:
	c=Rem(a,b).val;break;
    case 0x60:case 0x61:
	c=And(a,b).val;break;
    case 0x62:case 0x63:
	c=Or(a,b).val;break;
    }
}
/*
int main(){
    Memory_4 x,y;
    x.val=9999;
    y.val=1234;
    Debug(x);
    Debug(y);
    Debug(Add(x,y));
    Debug(Sub(x,y));
    Debug(Mul(x,y));
    Debug(Div(x,y));
    Debug(Rem(x,y));
    Debug(And(x,y));
    Debug(Or(x,y));
    return 0;
}

*/
