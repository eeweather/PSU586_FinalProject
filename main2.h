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

#define FALSE		0
#define TRUE		1

#define IF		0
#define ID		1
#define EX		2
#define MEM		3
#define WB		4

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

typedef struct inst
{
    char type;
    opcode_t opcode;
    int rs;
    int rt;
    int rd;
    int imm;
} inst_t;

// MIPS architecture struct
typedef struct mips_status
{
    bool debug; 	// whether program is in debug mode
    int mode;	// which simulation mode the program is in
    uint32_t pc;
    uint32_t npc;
    uint32_t pc_branch;
    uint32_t alu_temp;
    uint32_t mem_reg;
    uint32_t temp_pc;
    uint16_t count_total;
    uint16_t count_arith;
    uint16_t count_logic;
    uint16_t count_memory_access;
    uint16_t count_control_flow;
    bool     zero_flag;
    bool     jump_flag;
    bool     halt;
} mips_status_t;

// function prototypes
bool getDebug(int index, char* commands[]);
char* getInputFile(int index, char* commands[]);
char* getOutputFile(int index, char* commands[]);
int getMode(int index, char* commands[]);
FILE* openInputFile(const char* inputFile);
FILE* openOutputFile(const char* outputFile);
void closeFile(FILE* inputFile);

#endif
