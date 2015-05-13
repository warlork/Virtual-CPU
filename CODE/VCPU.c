/*
* Daniel Tudzi
* Main CPU Functions
* The functions necessary for CPU operation are here
* 2/7/2015
*/

#include <inttypes.h>
#include "linker.h"

/*
* Fetch() -- function to fetch instructions from start of memory 
* also splits IR to 2 seperate Instructions
*/
void Fetch(){
	MAR = PC;
	int i;
	for (i = 0; i < 4; i++){
		MBR = MBR << 8;
		MBR += mem[(MAR + i)];
	}

	PC += sizeof(REG_SIZE);
	IR = MBR;
	IR0 = IR >> 16;
	IR1 = IR >> 32;
}

/*
* Fetch() -- function to fetch instructions from start of memory
* also splits IR to 2 seperate Instructions
*/
void Decode(unsigned maskVal, uint16_t newIR_val){

	printf("\n*** DECODE ***\n");
	Imm = createMask(newIR_val, 4, 12);
	Rn = createMask(newIR_val, Rn_Startbit, Rn_Endbit);
	Rd = createMask(newIR_val, Rd_Startbit, Rd_Endbit);

	if (maskVal == 2 || maskVal == 3){
		printf("IMMEDIATE...\n\n");
		opCode = createMask(newIR_val, 12, 14);
		Immediate();
	}
	if (maskVal == DATA_PROC) {
		printf("DATA PROCESSING...\n");
		opCode = createMask(newIR_val, 8, 12);
		Data_Proc();
	}
	if (maskVal == STOP) {
		printf("STOPPING...\n");
		stop = 1;
	}
	if (maskVal == LOAD_STORE) {
		printf("----- LOAD / STORE ------\n");
		LdStrVal = createMask(newIR_val, 10, 12);
		currIR_val = newIR_val;
		LoadStore();
	}
	if (maskVal == COND_BRANCH) {
		printf("CONDITIONAL BRANCH...\n");
		unsigned cond = createMask(newIR_val, 8, 12);
		unsigned Rel_add = createMask(newIR_val, 0, 8);
		Cond_Branch(cond, Rel_add);
	}
	if (maskVal == PUSH_PULL) {
		printf("PUSH / PULL...\n");
		unsigned reg_List = createMask(newIR_val, 0, 8);
		unsigned r_bit = createMask(newIR_val, 8, 9);
		unsigned PP_bit = createMask(newIR_val, 11, 12);
		unsigned hi_bit = createMask(newIR_val, 10, 11);
		
		Push_Pull(reg_List, r_bit, PP_bit, hi_bit);
	}
	if (maskVal == UNCOND_BRANCH) {
		printf("... UNCONDITIONAL BRANCH...\n");
		unsigned offset = createMask(newIR_val, 0, 12);
		unsigned int k = createMask(newIR_val, 12, 13);
		Uncond_Branch(offset, k);
	}
}

/*
* Execute() -- This function creates the Instruction mask and send it 
* to the decode function to interperate it
*/
void Execute(uint16_t IR_val){	
	rtn_Mask = createMask(IR_val, 13, 16);
	Decode(rtn_Mask, IR_val);
}

/*
* Instruction() -- This function does and instruction one at a time
* Calls Fetch() and Execute when IRactive flag is 0; and only calls execute when IRactive flag is 1
*/
void Instruction(){

	if (IRactive == 0) {
		Fetch();
		IRactive = 1;
		Execute(IR0);
		Register();
	}
	else {
		IRactive = 0;
		Execute(IR1);
		Register();
	}
}

/*
* GO() -- This function calls the Instruction Function and runs the entire 
* program till gthe stop flag is set
*/
void GO(){
	while (stop == 0){
		Instruction();
	}
	printf("\nComplete !!!\n");
}

