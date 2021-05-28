///////////////////////////////////////////////////////////
//
//	main.c - Main functionality for pipeline simulator
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"
#include "mem_wb.h"

int main(int argc, char *argv[])
{

	bool debug = getDebug(argc, argv);
	char *input = getInputFile(argc, argv);
	char *output = getOutputFile(argc, argv);
	int mode = getMode(argc, argv);

	FILE *addressFile = openInputFile(input);  // check that we can open memory file to read from
	FILE *outputFile = openOutputFile(output); // check that we can open file to output to

	struct mips_status mips_status_struct; //initializing our MIPS status structure - add more components to this as needed in main.h

	mips_status_struct.mode = mode;
	mips_status_struct.debug = debug;
	mips_status_struct.npc = 0;
	mips_status_struct.pc = 0;		  //initialize the PC so that we can
	mips_status_struct.pc_branch = 4; //while coding checking, can otherwise initialize to zero
	mips_status_struct.jump_flag = FALSE;

	printf("initial value of pc is %d and initial value of pc_branch is %d\n", mips_status_struct.pc, mips_status_struct.pc_branch);

	int32_t registers[32];	//initialize our registers
	bool regChange[32];
	int32_t memory[1024];	//initialize the memory storage array
	bool memChange[1024];
	inst_t instructions[5]; //initializing instruction array

	int32_t branch_control_signal = 0; //mock branch control signal to use in IF stage for now. 0 means no branch to be taken, 1 means branch to be taken

	//registers[1] = 0x12345678; //while coding checking

	//all stages start locked except IF
	bool iflock = false;
	bool idlock = true;
	bool exlock = true;
	bool memlock = true;
	bool wblock = true;

	hazard_flag = false;

	for (int i = 0; i < 32; i++)
	{
		registers[i] = 0;		// initialize registers to 0
	}
	arrayMemImageFill(memory, addressFile); //to fill the memory with the file inputs in one loop (to avoid looping through the file many times)
	int i = 12;

	while (i > 0 /*mips_status_struct.halt!=TRUE*/)
	{
		if (!wblock)
		{
			printf("in WB\n");
			writeback_stage(instructions, &mips_status_struct, registers, regChange);
			printf("after the wb function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking

		}

		if (!memlock)
		{
			printf("in MEM\n");
			memory_stage(instructions, &mips_status_struct, registers, memory, memChange);
			printf("after the mem function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking
			wblock = false;
		}

		if (!exlock)
		{
			printf("in EX\n");
			execution_stage(instructions, &mips_status_struct, registers);
			printf("after the ex function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking
			memlock = false;
		}
		//printf("binary of instruction in ID: %x\n", instructions[ID].binary);

		if (!idlock)
		{
			printf("in ID\n");
			id_stage(instructions, &mips_status_struct, registers, memory, &hazard_flag);
			printf("after the id function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking
			exlock = false;
		}

		if (!iflock)
		{
			printf("in IF\n");
			inst_fetch(instructions, registers, memory, &mips_status_struct, branch_control_signal); //IF stage
			printf("after the if function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking
			idlock = false;
		}

		//printf("after the inst_fetch function, value of register 1: %x\n", registers[1]); //while coding checking
		//printf("after the inst_fetch function, pc is: %x\n", mips_status_struct.pc);	  //while coding checking
		printf("i is: %d\n", i);
		i--;
	}

//	printInstructionsByType(arith, logi, mem, ctl);
//	printRegPcStates(reg, regChange, pc);
//	printMemStates(mem, memChange);
//	printNoForwardingHazards(nfHazards, totalStall, totalCycles);
//	printForwardingHazards(fHazards, totalStall, totalCycles);
//	printSpeedupAchieved(nfCycles, fCycles);

	closeFile(addressFile); // close the input file
	closeFile(outputFile);	// close the output file

	return 0;
}

// reads command line and returns debug flag if set
bool getDebug(int index, char *commands[])
{

	for (int i = 1; i < index; i++)
	{

		if (strcmp(commands[i], "-d") == 0)
		{
			printf("\nDebug mode on\n\n");
			return ON;
		}
	}

	return OFF;
}

// reads command line and returns input filename if set
char *getInputFile(int index, char *commands[])
{

	for (int i = 1; i < index; i++)
	{

		if (strcmp(commands[i], "-i") == 0)
		{

			//check if the second argument is empty
			if (commands[i + 1] == NULL)
			{
				printf("\nError: Argument cannot be empty--terminating program\n\n");
				exit(1);
			}

			return commands[i + 1];
		}
	}

	return "input.txt";
}

// reads command line and returns output filename if set
char *getOutputFile(int index, char *commands[])
{

	for (int i = 1; i < index; i++)
	{

		if (strcmp(commands[i], "-o") == 0)
		{

			//check if the second argument is empty
			if (commands[i + 1] == NULL)
			{
				printf("\nError: Argument cannot be empty--terminating program\n\n");
				exit(1);
			}

			return commands[i + 1];
		}
	}

	return "output.txt";
}

int getMode(int index, char *commands[])
{

	for (int i = 1; i < index; i++)
	{

		if (strcmp(commands[i], "-m") == 0)
		{

			//check if the second argument is empty
			if (commands[i + 1] == NULL)
			{
				printf("\nError: Argument cannot be empty--terminating program\n\n");
				exit(1);
			}

			else if (strcmp(commands[i + 1], "0") == 0)
			{
				printf("\nMode: Functional simulation only\n\n");
				return FUNCTIONAL;
			}

			else if (strcmp(commands[i + 1], "1") == 0)
			{
				printf("\nMode: Simulation with timing\n\n");
				return TIMING;
			}

			else if (strcmp(commands[i + 1], "2") == 0)
			{
				printf("\nMode: Simulation with timing and data forwarding\n\n");
				return FORWARDING;
			}

			else
			{
				printf("\nError: Invalid mode argument--terminating program\n\n");
				exit(1);
			}
		}
	}

	return FUNCTIONAL;
}

// open input file to be read from
FILE *openInputFile(const char *inputFile)
{

	FILE *addressFile = fopen(inputFile, "r");
	if (!addressFile)
	{
		printf("\nUnable to read input file--terminating program\n\n");
		exit(1);
	}

	return addressFile;
}

// open output file to be written to
FILE *openOutputFile(const char *outputFile)
{

	FILE *addressFile = fopen(outputFile, "w");
	if (!addressFile)
	{
		printf("\nUnable to create output file--terminating program\n\n");
		exit(1);
	}

	return addressFile;
}

// close file
void closeFile(FILE *inputFile)
{

	int rc = fclose(inputFile);
	if (rc)
	{
		printf("\n\nUnable to close file\n\n");
		return;
	}
}

//fill memory array with input values from input file
void arrayMemImageFill(int32_t *memory_array, FILE *inputFile)
{

	int32_t maxArray = 1024;	//anything larger than 1024 from input file will be treated as an indication that the simulator is not functioning properly, professor gauranteed no larger than 1024 lines (4kB)
	char temp_input_string[20]; //chose 20 arbitarily by a bit of trial and error to make sure that everything on one line would get pulled in
	int i = 0;					// for incrementing through memory array
	int temp_number;			//for converting from string to number

	while (fgets(temp_input_string, sizeof(temp_input_string), inputFile) != NULL)
	{
		temp_number = (int32_t)strtol(temp_input_string, NULL, 16); // convert with number base 16
		memory_array[i] = temp_number;								//place each line into each mem location as number value
		printf("mem value %x from line %d\n", memory_array[i], i);
		i++;
	}

	if (i <= maxArray)
	{
		printf("End of input file has been reached. %d lines have been read into memory array.\n", i); //fyi for user
	}
	else
	{
		printf("Input file seems to have more than 1024 lines. Please debug. %d lines have been read into memory array.\n", i); //fyi for user
	}

	return;
}
