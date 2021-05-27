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

int main(int argc, char *argv[]){

	bool debug = getDebug(argc, argv);
	char* input = getInputFile(argc, argv);
	char* output = getOutputFile(argc, argv);
	int mode = getMode(argc, argv);

	FILE* addressFile = openInputFile(input);	// open memory file to read from
	FILE* outputFile = openOutputFile(output);	// open file to output to

////////////////////////// 


//declare a 4kB of mem storage (array?)
int32_t memory[1024]; 

//declare registers
int32_t registers[32];

// MIPS architecture struct
mips_status_t status;
status.debug = debug;
status.mode = mode;

//declare global clock counter, array[5], and circular buffer (5 entries)
//declare struct for currant instructions
inst_t instructions[5];


    //input data from memory image into array?

    //enter loop that continues until halt

    while (status.halt != true)
    {
        //increment counter
        //parse first instruction into binary
        //update array[5] with instruction
            //most first to IF, IF to ID, ID to EX, EX to MEM, MEM to WB
        //update circular buffer with instruction

        //check for RAW hazard, handel 

	// Non-forwarding: stall 1 tick if needed register is in MEM stage, 2 if in EX
	// Forwarding: if desired register is from non-load instruction, forward from its current stage
	//	if desired register is from load instruction: stall 1 tick then forward if load is previous
	//	instruction, or just forward if load is 2 instructions previous
	// -> I think this makes sense, but could be missing some scenarios [M]

        //execute instruction in EX
	// also if jump/branch to non-subsequent instruction, flush the pipeline (IF and ID stages) [M]

	memory_stage(instructions, &status, registers, memory);
	writeback_stage(instructions, &status, registers, memory);
    }

	// We might want to use the halt instruction as a signal to add NOPs to the end of the pipeline such that
	// the pipeline is flushed at the end of the program. Otherwise, depending on when we read the
	// HALT instruction, we might cut off some instructions still in the pipe. I think another option would be
	// to have the halt flag set during the WB stage of the halt instruction, so that the other instructions are
	// already completed by the time the flag's set [M]

	/*
	-That makes sense to me, I was picturing a kind of 'clean up' section outside of the loop, but that'd be easier
	to put it in the WB, I changed the sketch to reflect that [E]
	*/


	/////////////////////////////////////////////////////

	closeFile(addressFile);	// close the input file 
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

	return "input.txt";
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

	return "output.txt";
}

int getMode(int index, char* commands[]){

	for (int i = 1; i < index; i++){

		if (strcmp(commands[i], "-m") == 0){

			//check if the second argument is empty
			if(commands[i+1] == NULL){
				printf("\nError: Argument cannot be empty--terminating program\n\n");
				exit(1);
			}

			else if(strcmp(commands[i+1], "0") == 0){
				printf("\nMode: Functional simulation only\n\n");
				return FUNCTIONAL;
			}

			else if(strcmp(commands[i+1], "1") == 0){
				printf("\nMode: Simulation with timing\n\n");
				return TIMING;
			}

			else if(strcmp(commands[i+1], "2") == 0){
				printf("\nMode: Simulation with timing and data forwarding\n\n");
				return FORWARDING;
			}

			else{
				printf("\nError: Invalid mode argument--terminating program\n\n");
				exit(1);
			}
		} 
	} 

	return FUNCTIONAL;

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
void closeFile(FILE* inputFile) {

	int rc = fclose(inputFile);
	if (rc)
	{
		printf("\n\nUnable to close file\n\n");
		return;
	}
}
