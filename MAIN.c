/*
* Daniel Tudzi
* Virtual CPU
* Clarifying the relationship between the interface and virtual cpu.
* Declared memory as an array of bytes of 16kBytes
*
* 2/7/2015
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <inttypes.h>
#include "linker.h"

/**********************************************************
help()- Displays the Help menu
***********************************************************/
void help(){
	printf("\t d	dump memory\n");
	printf("\t g	g - run entire program\n");
	printf("\t l	load file into memory\n");
	printf("\t m	memory modify\n");
	printf("\t q	quit\n");
	printf("\t r	display registers\n");
	printf("\t t	trace - execute one instruction\n");
	printf("\t w	write file\n");
	printf("\t z	reset all registers to zero\n");
	printf("\t ?, h	display list of commands\n");
}

/**********************************************************
checkXinput()- Checks values if they are valid Hex digits
***********************************************************/
unsigned checkXinput(unsigned input[INPUT_MAX]){
	unsigned val;

	while (sscanf(input, "%x", &val) != 1){
		printf("Enter Valid Hex digit \n");
		fgets(input, sizeof(input), stdin);
	}
	return val;
}

/**********************************************************
getInput()- Fuction to get input form user
***********************************************************/
unsigned getInput(){
	unsigned val[INPUT_MAX];
	unsigned input1[INPUT_MAX];
	fgets(input1, sizeof(input1), stdin);
	sscanf(input1, "%s", &val);

	return val;
}

/**********************************************************
LoadFile()- Load a file form Disk
***********************************************************/
int LoadFile(void * memory, unsigned int max){
	FILE *fp;
	size_t bytes_read = 0;
	char input[INPUT_MAX];
	printf("Enter a name of a file: ");
	fgets(input, sizeof input, stdin);
	input[strlen(input) - 1] = '\0';

	fp = fopen(input, "rb");
	if (!fp){
		perror(" Load file error: ");
		return;
	}
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);         /*calc the size needed*/
	fseek(fp, 0, SEEK_SET);
	printf("Size of file is %d\n", size);
	printf("max is %d\n", max);
	if (size > max) {
		size = max;
		printf("File was truncated!!!\n");
	}
	bytes_read = fread(memory, 1, size, fp);

	fclose(fp);
	return bytes_read;
}

/**********************************************************
WriteFile()- Writes the contents of memory to Disk
***********************************************************/
void WriteFile(void * memory){
	FILE *fp;
	char input[INPUT_MAX];
	int writeBytes = 0; int i;
	printf("Enter a name of a file: ");
	fgets(input, sizeof(input), stdin);
	input[strlen(input) - 1] = '\0';

	if (strlen(input) > INPUT_MAX){
		printf("Filename too long!!!\n");
	}
	else if (strlen(input) <= INPUT_MAX){

		printf("Bytes to write: ");
		if (scanf(" %d", &writeBytes) == 0){
			printf("Not a valid integer!");
		}
		if (writeBytes >= MEM_SIZE){
			printf("Max size available is %d, please try again next time!", MEM_SIZE);
		}
		else{
			fp = fopen(input, "wb");
			if (!fp){
				perror(" Write file error: ");
				return;
			}
			size_t size = fwrite(memory, 1, writeBytes, fp);
			fclose(fp);
			printf("%d bytes successfully written to file!\n", size);
		}
		while ((i = getchar()) != '\n' && i != EOF);  // flush input buffer
	}

}

/**********************************************************
MemDump()- DUmps/ Displays memory from given location to offset
***********************************************************/
void MemDump(void *memptr, unsigned offset, unsigned length){

	unsigned char *buf = (unsigned char*)memptr;
	int i, j;
	/*Nested loop to run entire length from offset plus the lenght
	* Starting from the offset  and checking if offset is less than (offset+lenght)
	* to start the loop
	*/
	for (i = offset; i<length + offset && i <= MEM_SIZE; i += 16) {
		//printing offset value
		printf("%04x: ", i);
		//loop to print ASCII value 16 times
		for (j = 0; j < 16; j++){
			if (i + j < length + offset)
				printf("%02x ", buf[i + j]);
			else
				printf("   ");
			printf(" ");
		}
		//pad to allign ASCII characters with ASCII values.
		printf("\n");
		printf("     ");
		//loop to print ASCII character 16 times
		for (j = 0; j < 16; j++){

			if (i + j < length + offset)
				printf("  %c", isprint(buf[i + j]) ? buf[i + j] : '.');
			else
				printf("   ");
			printf(" ");
		}
		printf("\n");
	}

}

/**********************************************************
MemMod()- Modifies users specified memory location incrementally 
***********************************************************/
void MemMod(void *memptr){
	char checkXval[VAL_MAX]; char input1[VAL_MAX];
	unsigned returnVal; unsigned modOffset;
	printf("Starting address: ");
	fgets(checkXval, sizeof(checkXval), stdin);
	modOffset = checkXinput(checkXval);
	MemDump(&mem, modOffset, 1);
	printf("New value:  ");
	char *stop = ".";

	while (1){
		fgets(input1, sizeof input1, stdin);
		input1[strlen(input1) - 1] = '\0';

		if ((strcmp(input1, stop)) == 0){
			printf("user quit!!\n");
			return;
		}
		else{
			while (modOffset != MEM_SIZE){
				printf("offset value %x\n", modOffset);
				returnVal = checkXinput(input1);
				((unsigned char*)memptr)[modOffset] = returnVal;
				printf("Memory content after Mod :\n");
				MemDump(&mem, modOffset, 1);
				modOffset++;
				printf("Enter new value for next address ('.' to quit) : ");
			}
				printf("WARNING !!!");
				printf("End of Specified memory!! EXITING ");
				return;
		}
	}
}


/*
* Main function that controls and handles the choices to call functions for the user
* Also sets memory and Displays program information.
*/
int main()
{
	clearReg(); 
	resetFlags();
	char choice[100];
	char input[100]; char input2[100];
	char xcheckVal[100];

	printf("Daniel Tudzi\t"); printf(" 16kBytes Memory Virtual CPU\n\n");
	help();

	while (1){

		memset(choice, 0, sizeof(choice));
		printf(">> ");
		fgets(input, sizeof(input), stdin);
		sscanf(input, "%s", &choice);

		if (strlen(choice) > 1){
			printf("too many characters...!!!\n");
		}
		else{
			switch (tolower(*choice)) {

			case 'd':
					printf("\n****** Dump Memory ******\n\n");
					unsigned offset; unsigned length;
					printf("Enter value for offset in Hex: ");
					fgets(xcheckVal, sizeof(xcheckVal), stdin);
					offset = checkXinput(xcheckVal);
					printf("Enter number of bytes to display: ");
					fgets(xcheckVal, sizeof(xcheckVal), stdin);
					length = checkXinput(xcheckVal);
					printf("Offset is : %x and length is : %x \n", offset, length);
					MemDump(&mem, offset, length);
					break;
			case 'l':
					printf("****** Loading FIle to Memory ******\n\n");
					printf("size of memory is %d\n", sizeof(mem));
					int val = LoadFile(&mem, sizeof(mem));
					printf("Bytes read is %d (decimal) %X (hex)\n", val, val);
					break;
			case 'g':
					printf("g - run entire program\n");
					GO();
					break;
			case 'm':
					printf("****** Modifying Memory ******\n\n");
					MemMod(&mem);
					break;
			case 'q':
					printf("Quiting...!!!\n");
					return 1;
			case 'r':
					printf("****** Display Registers ******!\n\n");
					Register();
					break;
			case 't':
					printf("****** Trace (Single Step) ******\n\n");
					Instruction();
					break;
			case 'w':
					printf("writing file!\n\n");
					WriteFile(mem);
					break;
			case 'z':
					printf("reseting all registers to zero\n");
					break;
			case '?':
			case 'h':
					help();
					break;
			default:
					printf("Invalid choice\n");
			}
		}

	}
}


