#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE 	"input.txt"
#define OUTPUT_FILE	"output.txt"

#define BUFFERSIZE 	1024

#define OFF 		0
#define ON 		1

// typedefs and enums
typedef enum {
	ADD	= 0x0,
	ADDI	= 0x1,
	SUB	= 0x2,
	SUBI	= 0x3,
	MUL	= 0x4,
	MULI	= 0x5,
	OR	= 0x6,
	ORI	= 0x7,
	AND	= 0x8,
	ANDI	= 0x9,
	XOR	= 0xA,
	XORI	= 0xB,
	LDW	= 0xC,
	STW	= 0xD,
	BZ	= 0xE,
	BEQ	= 0xF,
	JR	= 0x10,
	HALT	= 0x11 
} opcode_t;

typedef struct r_type
{
	opcode_t opcode;
	int8_t rs;
	int8_t rt;
	int8_t rd;
} r_type_t;

typedef struct i_type
{
	opcode_t opcode;
	int8_t rs;
	int8_t rt;
	int32_t imm;
} i_type_t;


bool getDebug(int index, char* commands[]);
char* getInputFile(int index, char* commands[]);
char* getOutputFile(int index, char* commands[]);
uint32_t generate_r_type(r_type_t r_inst, int* count);
uint32_t generate_i_type(i_type_t i_inst, int* count);
FILE* openInputFile(const char* inputFile);
FILE* openOutputFile(const char* outputFile);
void closeFile(FILE* file);
opcode_t opcode_decode(char* opcode);
uint8_t reg_decode(char* reg);
r_type_t parse_r_type(char* rs_in, char* rt_in, char* rd_in);
i_type_t parse_i_type(char* rs_in, char* rt_in, char* imm_in);

int main(int argc, char** argv)
{	
	char* input = getInputFile(argc, argv);
	char* output = getOutputFile(argc, argv);
	FILE* inputFile = openInputFile(input);		// open file to input from
	FILE* outputFile = openOutputFile(output);	// open file to output to
	uint32_t instructions[1024];
	char temp1[100];
	char temp2[100];
	char temp3[100];
	char temp4[100];
	uint8_t opcode;
	int count = 0;

	r_type_t r_inst;
	i_type_t i_inst;

	bool debug = getDebug(argc, argv);

	while (!feof(inputFile))
	{
		char buf[BUFFERSIZE + 1];
	
		// get line from the file
		if (fgets(buf, 100, inputFile) == NULL)
		{
			break;
		}

		sscanf(buf, "%s %[^,], %[^,], %s", temp1, temp2, temp3, temp4);
		opcode = opcode_decode(temp1);
		if (opcode == ADD || opcode == SUB || opcode == MUL  
			|| opcode == OR || opcode == AND || opcode == XOR)
		{
			r_inst = parse_r_type(temp3, temp4, temp2);
			r_inst.opcode = opcode;
			instructions[count] = generate_r_type(r_inst, &count);
		}
		else
		{
			switch(opcode)
			{
				case ADDI:
				case SUBI:
				case MULI:
				case ORI:
				case ANDI:
				case XORI:
				case LDW:
				case STW:  i_inst = parse_i_type(temp3, temp2, temp4);
					break;
				case BEQ:  i_inst = parse_i_type(temp2, temp3, temp4);
					break;
				case BZ:   i_inst = parse_i_type(temp2, NULL, temp3);
					break;
		 		case JR:   i_inst = parse_i_type(temp2, NULL, NULL);
					break;
				case HALT: i_inst = parse_i_type(NULL, NULL, NULL);
					break;
				default:
					printf("\n===INVALID OPCODE===\n");
			}
			i_inst.opcode = opcode;
			instructions[count] = generate_i_type(i_inst, &count);
		}

	}


	printf("\n");
	for (int i = 0; i < count; i++)
	{
		if (debug)
		{
			printf("Instruction %d: %08X \n", i, instructions[i]);
		}
		fprintf(outputFile, "%08X \n", instructions[i]);
	}
	printf("\n");

	closeFile(outputFile);	// close the output file

	return 0;
}

// reads command line and returns debug flag if set
bool getDebug(int index, char* commands[]){

		for (int i = 1; i < index; i++){

			if (strcmp(commands[i], "-d") == 0){
				printf("\nDebug mode on\n\n");
				return ON;
			}
		}

		return OFF;

}

// reads command line and returns input filename if set
char* getInputFile(int index, char* commands[]){

	for (int i = 1; i < index; i++){

		if (strcmp(commands[i], "-i") == 0){

			//check if the second argument is empty
			if(commands[i+1] == NULL){
				printf("\nError: Argument cannot be empty--terminating program\n\n"); 
				exit(1);
			}

			return commands[i+1];
		}
	}

	return INPUT_FILE;
}

// reads command line and returns output filename if set
char* getOutputFile(int index, char* commands[]){

	for (int i = 1; i < index; i++){

		if (strcmp(commands[i], "-o") == 0){

			//check if the second argument is empty
			if(commands[i+1] == NULL){
				printf("\nError: Argument cannot be empty--terminating program\n\n"); 
				exit(1);
			}

			return commands[i+1];
		}
	}

	return OUTPUT_FILE;
}

uint32_t generate_r_type(r_type_t r_inst, int* count)
{
	opcode_t opcode = r_inst.opcode;
	uint8_t rs = r_inst.rs;
	uint8_t rt = r_inst.rt;
	uint8_t rd = r_inst.rd;

	if (rs < 0 || rs > 31)
	{
		printf("\nERROR (instruction %d): Invalid rs register\n", *count);
	}
	if (rt < 0 || rt > 31)
	{
		printf("\nERROR (instruction %d): Invalid rt register\n", *count);
	}
	if (rd < 0 || rd > 31)
	{
		printf("\nERROR (instruction %d): Invalid rd register\n", *count);
	}
	if (((opcode % 2) == 1) || (opcode == LDW) || (opcode == BZ) || (opcode == JR))
	{
		printf("\nERROR (instruction %d): Invalid R-type instruction opcode\n", *count);
	}

	uint32_t inst = (opcode << 26 | (rs & 0x1F) << 21 | (rt & 0x1F) << 16
			| (rd & 0x1F) << 11);

	(*count)++;

	return inst;
}

uint32_t generate_i_type(i_type_t i_inst, int* count)
{
	uint32_t inst = 0x0;
	uint32_t unsigned_imm;
	opcode_t opcode = i_inst.opcode;
	uint8_t rs = i_inst.rs;
	uint8_t rt = i_inst.rt;
	int32_t immediate = i_inst.imm;
	
	if (immediate >= 0)
	{
		unsigned_imm = immediate;
	}
	else
	{
		unsigned_imm = (1 << 16) + immediate;
	}

	if (rs < 0 || rs > 31)
	{
		printf("\nERROR (instruction %d): Invalid rs register\n", *count);
	}
	if (rt < 0 || rt > 31)
	{
		printf("\nERROR (instruction %d): Invalid rt register\n", *count);
	}
	if (immediate < -32768 || immediate > 32767)
	{
		printf("\nERROR (instruction %d): Invalid immediate field\n", *count);
	}
	if (((opcode >= 0 ) && (opcode < 12)) && (opcode % 2 == 0))
	{
		printf("\nERROR (instruction %d): Invalid I-type instruction opcode\n", *count);
	}

	else if (((opcode >= 0) && (opcode < 12)) || (opcode == LDW) || (opcode == STW) || (opcode == BEQ))
	{
		inst = (opcode << 26 | (rs & 0x1F) << 21 | (rt & 0x1F) << 16 | unsigned_imm);
	}

	else if (opcode == BZ) 
	{
		inst = (opcode << 26 | (rs & 0x1F) << 21 | unsigned_imm);
	}

	else if (opcode == JR) 
	{
		inst = (opcode << 26 | (rs & 0x1F) << 21);
	}

	else if (opcode == HALT)
	{
		inst = (opcode << 26);
	}

	(*count)++;

	return inst;
}

// open input file to be read from
FILE* openInputFile(const char* inputFile) {

	FILE *addressFile = fopen(inputFile, "r");
	if (!addressFile)
	{
		printf("\nUnable to read input file--terminating program\n\n");
		exit(1);
	}

	return addressFile;
}

// open output file to be written to
FILE* openOutputFile(const char* outputFile) {

	FILE *addressFile = fopen(outputFile, "w");
	if (!addressFile)
	{
		printf("\nUnable to create output file--terminating program\n\n");
		exit(1);
	}

	return addressFile;
}

// close file
void closeFile(FILE* file) {

	int rc = fclose(file);
	if (rc)
	{
		printf("\n\nUnable to close file--terminating program\n\n");
		exit(1);
	}

	return;
}

opcode_t opcode_decode(char* opcode)
{
	opcode_t opcode_int;

	if (strcmp(opcode, "ADD") == 0)
	{
		opcode_int = ADD;
	}
	else if (strcmp(opcode, "ADDI") == 0)
	{
		opcode_int = ADDI;
	}
	else if (strcmp(opcode, "SUB") == 0)
	{
		opcode_int = SUB;
	}
	else if (strcmp(opcode, "SUBI" )== 0)
	{
		opcode_int = SUBI;
	}
	else if (strcmp(opcode, "MUL") == 0)
	{
		opcode_int = MUL;
	}
	else if (strcmp(opcode, "MULI") == 0)
	{
		opcode_int = MULI;
	}
	else if (strcmp(opcode, "OR") == 0)
	{
		opcode_int = OR;
	}
	else if (strcmp(opcode, "ORI") == 0)
	{
		opcode_int = ORI;
	}
	else if (strcmp(opcode, "AND") == 0)
	{
		opcode_int = AND;
	}
	else if (strcmp(opcode, "ANDI") == 0)
	{
		opcode_int = ANDI;
	}
	else if (strcmp(opcode, "XOR") == 0)
	{
		opcode_int = XOR;
	}
	else if (strcmp(opcode, "XORI") == 0)
	{
		opcode_int = XORI;
	}
	else if (strcmp(opcode, "LDW") == 0)
	{
		opcode_int = LDW;
	}
	else if (strcmp(opcode, "STW") == 0)
	{
		opcode_int = STW;
	}
	else if (strcmp(opcode, "BZ") == 0)
	{
		opcode_int = BZ;
	}
	else if (strcmp(opcode, "BEQ") == 0)
	{
		opcode_int = BEQ;
	}
	else if (strcmp(opcode, "JR") == 0)
	{
		opcode_int = JR;
	}
	else if (strcmp(opcode, "HALT") == 0)
	{
		opcode_int = HALT;
	}
	else
	{
		printf("\n===INVALID INSTRUCTION===\n");
	}

	return opcode_int;
}

uint8_t reg_decode(char* reg)
{
	uint8_t reg_int;
	uint32_t reg_int_temp;
	sscanf(reg, "R%u", &reg_int_temp);
	if (reg_int_temp > 31)
	{
		printf("\n===INVALID REGISTER===\n");
	}
	reg_int = (uint8_t)reg_int_temp;

	return reg_int;	
}

r_type_t parse_r_type(char* rs_in, char* rt_in, char* rd_in)
{
	r_type_t inst;

	inst.rs = reg_decode(rs_in);
	inst.rt = reg_decode(rt_in);
	inst.rd = reg_decode(rd_in);

	return inst;
}

i_type_t parse_i_type(char* rs_in, char* rt_in, char* imm_in)
{
	i_type_t inst;

	inst.rs = (rs_in == NULL) ? 0 : reg_decode(rs_in);
	inst.rt = (rt_in == NULL) ? 0 : reg_decode(rt_in);
	inst.imm = (imm_in == NULL) ? 0 : atoi(imm_in);

	return inst;
}


