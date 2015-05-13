/*
* Daniel Tudzi
* linker.h
* Header that holds all the global variable 
* 2/7/2015
*/
#define MEM_SIZE 0x4000 //Maximum memory size in Hex
#define REG_SIZE 16		// Size of Register[]

#define TRUE 1
#define FALSE 0
#define MAX32 32
#define neg_Bit 0xF0000000 //bit to mask or check if negative or sign
#define INPUT_MAX 80 //input array size define
#define VAL_MAX 80 //value array size define
#define mem_MASK 0x3FFF
#define Base_Ptr 0x00003FDF; //Pointer for Stack

//Rn & Rd mask values
#define Rn_mask 0x00F0
#define Rd_mask 0x000F
#define Rn_Startbit 0x4
#define Rn_Endbit 0x8
#define Rd_Startbit 0x0
#define Rd_Endbit 0x4


//Special Purpose registers
#define PC reg[15]
#define LR reg[14]
#define SP reg[13]

//Push or Pull determinants
#define PUSH 0
#define PULL 1

//Instructions defines
#define DATA_PROC 0x00
#define LOAD_STORE 0x01
#define IMMEDIATE 0x2 || 0x03
#define COND_BRANCH 0x04
#define PUSH_PULL 0x05
#define UNCOND_BRANCH 0x06
#define STOP 0x07

//Immediate Operations OpCodes
#define opCode_MOV 0x00
#define opCode_CMP 0x01
#define opCode_ADD 0X02
#define opCode_SUB 0x03

//Data Processing Instruction defines
#define AND 0x0000
#define EOR 0x0001
#define SUB 0x0002
#define SXB 0x0003
#define ADD 0x0004
#define ADC 0x0005
#define LSR 0x0006
#define LSL 0x0007
#define TST 0x0008
#define TEQ 0x0009
#define CMP 0x000A
#define ROR 0x000B
#define ORR 0x000C
#define MOV 0x000D
#define BIC 0x000E
#define MVM 0x000F

unsigned char mem[MEM_SIZE]; //declaring memory
uint32_t reg[REG_SIZE]; //declaring the register size

uint16_t Rd; //declaring Rd as the destination value
uint16_t Rn; //declaring Rn as the source or processing value

uint32_t IR; //(32-bits) Instruction Register
uint16_t IR0, IR1; //(16-bits) Instruction Registers split in 2 from IR
uint16_t currIR_val; // used to assign the effective IR instruction

uint32_t MBR; //Memory Buffer Register
uint32_t MAR; //Memory Address Register
uint32_t ALU; //ALU Register

unsigned IR0_mask; // And this with IR0
unsigned IR1_mask; // And this with IR1

unsigned rtn_Mask; //Mask value passed to decode from the Exectute Fuction; determines the Instruction
unsigned LdStrVal; //Variable for Load/Store word/register 
unsigned opCode; // Operation Code for Immediate Instruction

unsigned Imm; // variable hold the immediate value to be processed

//CCR flags
int zero;
int sign;
int carry;
int stop;
int IRactive;

int byteWord;



