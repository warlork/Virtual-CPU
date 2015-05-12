/*
* Daniel Tudzi
* Vcpu_Funct.c
* The functions necessary for CPU operation are here
* 2/7/2015
*/
#include <inttypes.h>
#include "linker.h"


void Register(){
	int i;
	for (i = 0; i < REG_SIZE; i++){
		printf("r%02d:%08x\t", i, reg[i]);
	}
	printf("MAR:%08lx\t", MAR); printf("MBR:%08lx\t", MBR); printf("IR :%08lx\t", IR);
	printf("\nZCS:IRST:\n%d%d%d:%d %d\n", zero, carry, sign, IRactive, stop);
	printf("IR0: %04x\t", IR0); printf("IR1: %04x\n", IR1);
	return;
}

/*******************************************************************
* load_Mem() function to load byte/word from memory to register
*******************************************************************/
void load_Mem(uint16_t IR_val, int Bval){

	if (byteWord){
		printf("... loading byte ...\n");
		MAR = reg[Rn];
		MBR = mem[MAR];
		reg[Rd] = MBR;
	}
	else{
		printf("... loading word ...\n");
		MAR = reg[Rn];
		int i;
		for (i = 0; i < 4; i++){
			MBR = MBR << 8;
			MBR += mem[MAR++];
		}
		reg[Rd] = MBR;
	}
}

/*******************************************************************
* store_Mem() function to store values from set registers to memory
*******************************************************************/
void store_Mem(uint16_t IR_val, int Bval){
	MAR = reg[Rn];
	if (byteWord){
		printf("... storing byte ...\n");
		MBR = reg[Rd];
		mem[MAR] = MBR;
	}
	else{
		printf("... storing word ...\n");
		MBR = reg[Rd];
		printf("MBR value to store is %x\n", MBR);
		int i;
		for (i = 3; i >= 0; --i){
			mem[MAR++] = MBR >> 8 * i;
		}
	}
}

/*******************************************************************
* clearReg() function to clear the contents of the registers
*******************************************************************/
void clearReg(){
	//memset(reg, 0, REG_SIZE * sizeof(reg));
	int i;
	for (i = 0; i < REG_SIZE; i++){
		reg[i] = 0;
	}

}

/**********************************************************
*resetFlags() Flags function to reset the flags to 0
***********************************************************/
void resetFlags(){
	sign = 0;
	IRactive = 0;
	stop = 0;
	zero = 0;
	carry = 0;
}

/*******************************************************************
createMask()- creates mask from the given value from the
set beginning bit to the end bit
********************************************************************/
unsigned createMask(unsigned short value, int begin, int end){

	uint16_t mask = (1 << (end - begin)) - 1;
	return (value >> begin) & mask;
}

/**********************************************************
iscarry()- determine if carry is generated by addition: op1+op2+C
C can only have value of 1 or 0.
***********************************************************/
int iscarry(unsigned long op1, unsigned long op2, unsigned C){
	if ((op2 == MAX32) && (C == 1))
		return(1); // special case where op2 is at MAX32
	return((op1 > (MAX32 - op2 - C)) ? 1 : 0);
}

/**********************************************************
is_Sign()- determine if negetive is generated by subtraction
***********************************************************/
void is_Sign(uint32_t alu){
	if ((alu & 0xF0000000) == 0){
		sign = 1;
		zero = 0;
	}
	else{
		sign = 0;
		zero = 1;
	}
}

//do_Sub -- Subtruction function; Also sets sign, carry & zero flags
void do_Sub(unsigned num1, unsigned num2){
	reg[Rd] = num1;
	ALU = reg[Rd] + (~num2 + 1);
	is_Sign(ALU);
	carry = iscarry(ALU, num2, carry);
}

//do_Add -- Addition function; Also sets sign, carry & zero flags
void do_Add(unsigned num1, unsigned num2){
	reg[Rd] = num1;
	ALU = reg[Rd] + num2;
	is_Sign(ALU);
	carry = iscarry(ALU, num2, carry);
	reg[Rd] = ALU;
}



