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
	int16_t imm;
} i_type_t;


bool getDebug(int index, char* commands[]);
char* getInputFile(int index, char* commands[]);
char* getOutputFile(int index, char* commands[]);
FILE* openInputFile(const char* inputFile);
FILE* openOutputFile(const char* outputFile);
void closeFile(FILE* file);
char* opcode_encode(opcode_t opcode);
r_type_t parse_r_type(uint32_t instruction);
i_type_t parse_i_type(uint32_t instruction);
void print_i_type(bool debug, i_type_t instruction, FILE* file);
void print_r_type(bool debug, r_type_t instruction, FILE* file);


int main(int argc, char** argv)
{	
	char* input = getInputFile(argc, argv);
	char* output = getOutputFile(argc, argv);
	bool debug = getDebug(argc, argv);
	FILE* inputFile = openInputFile(input);		// open file to input from
	FILE* outputFile = openOutputFile(output);	// open file to output to
	uint32_t instructions[1024];
	int count = 0;
	uint32_t instruction;
	opcode_t opcode;
	r_type_t r_inst;
	i_type_t i_inst;

	while (!feof(inputFile))
	{
		// get line from the file
		if (fscanf(inputFile, "%x", &instruction) <= 0)
		{
			printf("\nEnd of file!\n");
			break;
		}

		opcode = (instruction >> 26);
		if (opcode > 17 || opcode < 0)
		{
			printf("===Invalid Opcode!===");
		}
		else if (opcode == ADD || opcode == SUB || opcode == MUL  
			|| opcode == OR || opcode == AND || opcode == XOR)
		{
			r_inst = parse_r_type(instruction);
			print_r_type(debug, r_inst, outputFile);
		}
		else
		{
			i_inst = parse_i_type(instruction);
			print_i_type(debug, i_inst, outputFile);
		}

	}


	if (debug)
	{
		printf("\n");
		for (int i = 0; i < count; i++)
		{
			printf("Instruction %d: %08X \n", i, instructions[i]);
		}
		printf("\n");
	}

	for (int i = 0; i < count; i++)
	{
		fprintf(outputFile, "%08X \n", instructions[i]);
	}

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

r_type_t parse_r_type(uint32_t instruction)
{
	r_type_t r_inst;
	opcode_t opcode;

	opcode = (instruction >> 26);
	if (opcode > 17 || opcode < 0)
	{
		printf("===Invalid Opcode!===");
	}
	r_inst.opcode = opcode;

	r_inst.rs = (instruction & 0x03E00000) >> 21;
	r_inst.rt = (instruction & 0x001F0000) >> 16;
	r_inst.rd = (instruction & 0x0000F800) >> 11;

	return r_inst;
}

i_type_t parse_i_type(uint32_t instruction)
{
	i_type_t i_inst;
	opcode_t opcode;

	opcode = (instruction >> 26);
	if (opcode > 17 || opcode < 0)
	{
		printf("===Invalid Opcode!===");
	}
	i_inst.opcode = opcode;

	i_inst.rs = (instruction & 0x03E00000) >> 21;
	i_inst.rt = (instruction & 0x001F0000) >> 16;
	i_inst.imm = instruction & 0xFFFF;

	return i_inst;
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

char* opcode_encode(opcode_t opcode)
{
	static char opcode_str[5];

	switch(opcode)
	{
		case ADD:	strcpy(opcode_str, "ADD");
				break;
		case ADDI:	strcpy(opcode_str, "ADDI");
				break;
		case SUB:	strcpy(opcode_str, "SUB");
				break;
		case SUBI:	strcpy(opcode_str, "SUBI");
				break;
		case MUL:	strcpy(opcode_str, "MUL");
				break;
		case MULI:	strcpy(opcode_str, "MULI");
				break;
		case OR:	strcpy(opcode_str, "OR");
				break;
		case ORI:	strcpy(opcode_str, "ORI");
				break;
		case AND:	strcpy(opcode_str, "AND");
				break;
		case ANDI:	strcpy(opcode_str, "ANDI");
				break;
		case XOR:	strcpy(opcode_str, "XOR");
				break;
		case XORI:	strcpy(opcode_str, "XORI");
				break;
		case LDW:	strcpy(opcode_str, "LDW");
				break;
		case STW:	strcpy(opcode_str, "STW");
				break;
		case BZ:	strcpy(opcode_str, "BZ");
				break;
		case BEQ:	strcpy(opcode_str, "BEQ");
				break;
		case JR:	strcpy(opcode_str, "JR");
				break;
		case HALT:	strcpy(opcode_str, "HALT");
				break;
	}

	return opcode_str;
}

void print_r_type(bool debug, r_type_t instruction, FILE* file)
{
	opcode_t opcode = instruction.opcode;
	uint8_t rd = instruction.rd;
	uint8_t rs = instruction.rs;
	uint8_t rt = instruction.rt;

	if (debug)
	{
		printf("%s R%d, R%d, R%d\n", opcode_encode(opcode), rd, rs, rt);
	}
	fprintf(file, "%s R%d, R%d, R%d\n", opcode_encode(opcode), rd, rs, rt);

	return;
}

void print_i_type(bool debug, i_type_t instruction, FILE* file)
{
	opcode_t opcode = instruction.opcode;
	uint8_t rs = instruction.rs;
	uint8_t rt = instruction.rt;
	int16_t imm = instruction.imm;

	if (opcode == ADDI || opcode == SUBI || opcode == MULI
		|| opcode == ORI || opcode == ANDI || opcode == XORI
		|| opcode == LDW || opcode == STW)
	{
		if (debug)
		{
			printf("%s R%d, R%d, %d\n", opcode_encode(opcode), rt, rs, imm);
		}
		fprintf(file, "%s R%d, R%d, %d\n", opcode_encode(opcode), rt, rs, imm);
	}
	else if (opcode == BZ)
	{
		if (debug)
		{
			printf("%s R%d, %d\n", opcode_encode(opcode), rs, imm);
		}
		fprintf(file, "%s R%d, %d\n", opcode_encode(opcode), rs, imm);
	}
	else if (opcode == BEQ)
	{
		if (debug)
		{
			printf("%s R%d, R%d, %d\n", opcode_encode(opcode), rs, rt, imm);
		}
		fprintf(file, "%s R%d, R%d, %d\n", opcode_encode(opcode), rs, rt, imm);
	}
	else if (opcode == JR)
	{
		if (debug)
		{
			printf("%s R%d\n", opcode_encode(opcode), rs);
		}
		fprintf(file, "%s R%d\n", opcode_encode(opcode), rs);
	}
	else if (opcode == HALT)
	{
		if (debug)
		{
			printf("%s\n", opcode_encode(opcode));
		}
		fprintf(file, "%s\n", opcode_encode(opcode));
	}

	return;
}
