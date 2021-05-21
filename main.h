#ifndef _HEADER_H
#define _HEADER_H

// libraries and includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// definitions
#define FUNCTIONAL 	0	// simulator modes
#define TIMING		1
#define FORWARDING 	2

#define OFF 		0	// general definitions
#define ON		1

// typedefs and enums
typedef enum {
    // Arithmetic Instructions
	ADD	= 0x0,
	ADDI	= 0x1,
	SUB	= 0x2,
	SUBI	= 0x3,
	MUL	= 0x4,
	MULI	= 0x5,
    // Logical Instructions
	OR	= 0x6,
	ORI	= 0x7,
	AND	= 0x8,
	ANDI	= 0x9,
	XOR	= 0xA,
	XORI	= 0xB,
    // Load/store
	LDW	= 0xC,
	STW	= 0xD,
    // Branch
	BZ	= 0xE,
	BEQ	= 0xF,
	JR	= 0x10,
    // Halt Op
	HALT	= 0x11 
} opcode_t;

struct mips_status
{
	uint32_t pc;
	uint32_t pc_branch;
};

// function prototypes for main.c
int getDebug(int index, char* commands[]);
char* getInputFile(int index, char* commands[]);
char* getOutputFile(int index, char* commands[]);
int getMode(int index, char* commands[]);
FILE* openInputFile(const char* inputFile);
FILE* openOutputFile(const char* outputFile);
void closeFile(FILE* inputFile);
void arrayMemImageFill(int32_t* memory_array, FILE* inputFile);

//function prototypes for if.c
void inst_fetch(int32_t* registers, int32_t* memory, struct mips_status* status_struct, int32_t branch_signal);

#endif
