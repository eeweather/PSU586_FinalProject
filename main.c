///////////////////////////////////////////////////////////
//
//	main.c - Main functionality for pipeline simulator
//
//	ECE 586 Spring 2021
//	Final Project
//
//  Description:  Program models a simplified version of the 
//  MIPS ISA called MIPS-lite and the in-order 5-stage pipeline.
//
//  To run using a specified mode use the "-m" command.
//  ./main -m <mode number>
//  
//  modes:
//          "0" - Functional simulation 
//          "1" - Simulation with timing 
//          "2" - Simulation with timing and data forwarding
//
///////////////////////////////////////////////////////////

#include "main.h"
#include "formatting.h"

int main(int argc, char *argv[])
{

	bool debug = getDebug(argc, argv);
	char *input = getInputFile(argc, argv);
	char *output = getOutputFile(argc, argv);
	int mode = getMode(argc, argv);

	FILE *addressFile = openInputFile(input);  // check that we can open memory file to read from
	FILE *outputFile = openOutputFile(output); // check that we can open file to output to

	mips_status_t status; //initializing our MIPS status structure - add more components to this as needed in main.h
	initialize_status(&status, mode, debug);

	int32_t registers[32];	//initialize our registers
	bool 	regChange[32];
	int32_t memory[1024];	//initialize the memory storage array
	bool 	memChange[1024];
	inst_t	instructions[5]; //initializing instruction array

	int32_t branch_control_signal = 0; //mock branch control signal to use in IF stage for now. 0 means no branch to be taken, 1 means branch to be taken

	initialize_registers(registers);
	initialize_changeArrays(memChange, regChange);
	initialize_instructions(instructions);

	hazard_flag = false; //initialize hazard flag

	//initialize forwarding flag info (register and pipeline stage)
	forward_stage_t forward_stage_flag = NO_FWDH;
	forward_reg_t forward_reg_flag;


	arrayMemImageFill(memory, addressFile); //to fill the memory with the file inputs in one loop (to avoid looping through the file many times)

	while (status.halt == false)
	{
		writeback_stage(instructions, &status, registers, regChange);
		memory_stage(instructions, &status, registers, memory, memChange);
        if (status.halt == true){
			printf("Halting\n");
            break;}
		execution_stage(instructions, &status, registers, &forward_stage_flag, &forward_reg_flag);
		id_stage(instructions, &status, registers, memory, &hazard_flag, &forward_stage_flag, &forward_reg_flag);
		inst_fetch(instructions, registers, memory, &status, branch_control_signal, &hazard_flag); //IF stage

//		char* str[1024];
//		gets(str);
	}
    
    
	printInstructionsByType(status.count_arith, status.count_logic, status.count_memory_access, status.count_control_flow, outputFile);
	printRegPcStates(registers, regChange, status.pc, outputFile);
	printMemStates(memory, memChange, outputFile);
	printHazards(status.count_hazards, status.count_stall, status.cycles, outputFile);

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

void initialize_status(mips_status_t* status, int mode, bool debug)
{
	status->debug 			= debug;
	status->mode 			= mode;
	status->pc 			= 0;		  //initialize the PC so that we can
	status->npc 			= 0;
	status->pc_branch 		= 4; //while coding checking, can otherwise initialize to zero
	status->alu_temp 		= 0;
	status->prior_alu_temp 		= 0;
	status->mem_reg 		= 0;
	status->temp_pc 		= 0;
	status->jump_flag 		= false;
	status->count_total 		= 0; //initialize counts to zero
	status->count_arith 		= 0;
	status->count_logic 		= 0;
	status->count_memory_access 	= 0;
	status->count_control_flow 	= 0;
	status->count_stall 		= 0;
	status->flushcount			= 0;
	status->zero_flag 		= false;
	status->jump_flag 		= false;
	status->halt 			= false;

	return;
}

void initialize_instructions(inst_t* instructions)
{
	for (int index = 0; index < 5; index++)
	{
    	instructions[index].binary 		= 0; //maybe dont need
   		instructions[index].type 		= 'r';
   		instructions[index].opcode 		= ADD;
   	 	instructions[index].rs 			= 0;  //removed separate rs and rt for instruction types
   	 	instructions[index].rt 			= 0;
   	 	instructions[index].rd 			= 0;
   	 	instructions[index].imm 		= 0;
   	 	instructions[index].valA 		= 0;
   	 	instructions[index].valB 		= 0;
		if (index > 0)
		{	
  	 	 	instructions[index].nop 	= true;
		}
		else
		{
			instructions[index].nop		= false;
		}
 	 	instructions[index].branch 		= false;
   	 	instructions[index].halt 		= false;
	}
}

void initialize_changeArrays(bool* memChange, bool* regChange)
{
	for (int index = 0; index < 1024; index++)
	{
		memChange[index] = false;
	}

	for (int index = 0; index < 32; index++)	// initialize regChange array to false flags 
	{
		regChange[index] = false;
	}
}

void initialize_registers(int32_t* registers)
{
	for (int index = 0; index < 32; index++)
	{
		registers[index] = 0; //set registers to 0 to begin
	}
}
