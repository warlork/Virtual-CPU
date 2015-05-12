/*
* Daniel Tudzi
* Instructs.c
* The Instruction Functions necessary for CPU operation are here
* 2/7/2015
*/
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include "linker.h"

/**********************************************************
Data_Proc()- Data Processing Function
***********************************************************/
void Data_Proc(){
	if (opCode == AND){
		printf("AND\n");
		ALU = reg[Rd] & reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
	if (opCode == EOR){
		printf("EOR\n");
		ALU = reg[Rd] ^ reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
	if (opCode == SUB){
		printf("SUB\n");
		do_Sub(reg[Rd], reg[Rn]);
		reg[Rd] = ALU;
	}
	if (opCode == SXB){
		printf("SXB\n");
		reg[Rd] = (signed)reg[Rn];
		is_Sign(reg[Rd]);
	}
	if (opCode == ADD){
		printf("ADD\n");
		do_Add(reg[Rd], reg[Rn]);
	}
	if (opCode == ADC){
		printf("ADC\n");
		do_Add(reg[Rd], reg[Rn]);
		reg[Rd] += carry;
	}
	if (opCode == LSR){
		printf("LSR\n");
		ALU = reg[Rd] >> reg[Rn];
		is_Sign(ALU);
		carry = iscarry(ALU, reg[Rn], carry);
		reg[Rd] = ALU;
	}
	if (opCode == LSL){
		printf("LSL\n");
		ALU = reg[Rd] << reg[Rn];
		is_Sign(ALU);
		carry = iscarry(ALU, reg[Rn], carry);
		reg[Rd] = ALU;
	}
	if (opCode == TST){
		printf("TST\n");
		ALU = reg[Rd] & reg[Rn];
		is_Sign(ALU);
	}
	if (opCode == TEQ){
		printf("TEQ\n");
		ALU = reg[Rd] ^ reg[Rn];
		is_Sign(ALU);
	}
	if (opCode == CMP){
		printf("CMP\n");
		do_Sub(reg[Rd], reg[Rn]);
	}
	if (opCode == ROR){
		printf("ROR\n");
		ALU = (reg[Rd] >> reg[Rn]) | (reg[Rd] << (sizeof(reg[Rd]) * CHAR_BIT - reg[Rn]));
		is_Sign(ALU);
		carry = iscarry(ALU, reg[Rn], carry);
		reg[Rd] = ALU;
	}
	if (opCode == ORR){
		printf("ORR\n");
		ALU = reg[Rd] | reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
	if (opCode == MOV){
		printf("MOV\n");
		ALU = reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
	if (opCode == BIC){
		printf("BIC\n");
		ALU = reg[Rd] & ~reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
	if (opCode == MVM){
		printf("MVN\n");
		ALU = ~reg[Rn];
		is_Sign(ALU);
		reg[Rd] = ALU;
	}
}

/**********************************************************
LoadStore() -- Load/ Store Instruction Function
***********************************************************/
void LoadStore(){
	if (LdStrVal == 0){
		printf("L bit is 0, doing Store to mem\n ");
		printf("B bit is 0, doing transfer word\n ");
		byteWord = 0;
		store_Mem(currIR_val, byteWord);
	}
	if (LdStrVal == 1){
		printf("L bit is 0, doing Store to mem\n ");
		printf("B bit is 1, doing transfer byte\n ");
		byteWord = 1;
		store_Mem(currIR_val, byteWord);
	}
	if (LdStrVal == 2){
		printf("L bit is 1, doing Load from mem\n ");
		printf("B bit is 0, doing transfer word\n ");
		byteWord = 0;
		load_Mem(currIR_val, byteWord);
	}
	if (LdStrVal == 3){
		printf("L bit is 1, doing Load from mem\n ");
		printf("B bit is 1, doing transfer byte\n ");
		byteWord = 1;
		load_Mem(currIR_val, byteWord);
	}
}

/**********************************************************
Immediate() -- Immediate Branch Instruction Function
***********************************************************/
void Immediate(){

	switch (opCode){
	case 0:
		printf("--MOV\n");
		reg[Rd] = Imm;
		break;
	case 1:
		printf("--CMP\n");
		printf("RD vAL = %x\n", reg[Rd]);
		do_Sub(reg[Rd], Imm);
		break;
	case 2:
		printf("--ADD\n");
		do_Add(reg[Rd], Imm);
		break;
	case 3:
		printf("--SUB\n");
		do_Sub(reg[Rd], Imm);
		reg[Rd] = ALU;
		break;
	default:printf("ERROR: Invalid opCode!!\n");
	}
}

/**********************************************************
Cond_Branch() -- Conditional Branch Instruction Function
***********************************************************/
void Cond_Branch(unsigned Cond, unsigned Rel_add){
	char relAdd = Rel_add;
	PC -= 2;
	IRactive = 0;
	if (Cond == 0) {
		PC = PC + relAdd;
	}
	if (Cond == 1) {
		if (zero == 0) {
			PC += relAdd;
			printf("PC val is %x\n", PC);
			IR = 0;
			IRactive = 0;
		}
	}
	if (Cond == 2) {
		PC = PC + relAdd;
	}
	if (Cond == 3) {
		PC = PC + relAdd;
	}
	if (Cond == 4) {
		PC = PC + relAdd;
	}
	if (Cond == 5) {
		PC = PC + relAdd;
	}
	if (Cond == 8) {
		PC = PC + relAdd;
	}
	if (Cond == 9) {
		PC = PC + relAdd;
	}
}

/**********************************************************
Push_Pull() -- Pull / Push Instruction Function
***********************************************************/
void Push_Pull(unsigned reg_List, unsigned r_bit, unsigned PP_bit, unsigned hi_bit){
	int i, j;
	//PUSH INSTRUCTION
	if (PP_bit == PUSH){
		printf("PUSH\n");
		MAR = SP;
		if (hi_bit == 0){
			printf("Pushing Low Registers\n");
			for (i = 8; i >= 0; i--){
				if (((reg_List >> i) & 1) == 1){
					MBR = reg[i];
					for (j = 0; j < 4; j++){
						--MAR;
						MAR = MAR & Base_Ptr;
						mem[MAR] = (MBR >> (8 * j));
					}
				}
			}
			if (r_bit == 1){
				//Extra Push
				printf("There is an extra Push");
				LR = PC;
				MBR = LR;
				for (j = 0; j < 4; j++){
					--MAR;
					MAR = MAR & Base_Ptr;
					mem[MAR] = (MBR >> (8 * j));
				}
			}
		}
		if (hi_bit == 1){
			printf("Pushing High Registers\n");
			for (i = 8; i >= 0; i--){
				if (((reg_List >> i) & 1) == 1){
					MBR = reg[(i + 8)];
					for (j = 0; j < 4; j++){
						--MAR;
						MAR = MAR & Base_Ptr;
						mem[MAR] = (MBR >> (8 * j));
					}
				}
			}
		}
	}

	//PULL INSTRUCTION
	//LOW Registers
		if (hi_bit == 0) {
			if (r_bit == 0){
				for (i = 0; i < 8; i++){
						printf("hello");
						if (((reg_List >> i) & 1) == 1){
							for (j = 0; j < 4; j++){
									MBR = MBR << 8;
									MBR += ((unsigned char*)mem)[MAR++];
								}
									reg[i] = MBR;
						}
				}
			}
		}
		else if (r_bit == 1){
			for (j = 0; j < 4; j++){
				MBR = MBR << 8;
				MBR += ((unsigned char*)mem)[MAR++];
			}
				reg[LR] = MBR;
			for (i = 0; i < 8; i++){
				for (j = 0; j < 4; j++){
					MBR = MBR << 8;
					MBR += ((unsigned char*)mem)[MAR++];
				}
				reg[i] = MBR;	
			}	
		}
		//PULL HIGH Registers
		else if (hi_bit == 1){
			if (r_bit == 0){
				for (i = 0; i < 8; i++){
							if (((reg_List >> i) & 1) == 1){
								for (j = 0; j < 4; j++)
								{
									MBR = MBR << 8;
									MBR += ((unsigned char*)mem)[MAR++];
								}
								reg[i + 8] = MBR;
							}
						}
			}
			else if (r_bit == 1){
				for (j = 0; j < 4; j++)
						{
							MBR = MBR << 8;
							MBR += ((unsigned char*)mem)[MAR++];
						}
						reg[LR] = MBR;
						for (i = 0; i < 8; i++){
							for (j = 0; j < 4; j++)
							{
								MBR = MBR << 8;
								MBR += ((unsigned char*)mem)[MAR++];
							}
							reg[i + 8] = MBR;
						}
			}
			
		}
		
		SP = MAR;
	}
	
		


/**********************************************************
Uncond_Branch() -- Unconditional Branch Instruction Function
***********************************************************/
void Uncond_Branch(unsigned offset, unsigned int k){
	if (k == 0){
		PC = offset;
	}
	if (k == 1){
		LR = PC;
		PC = offset;
	}
}
