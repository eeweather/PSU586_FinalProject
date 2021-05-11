///////////////////////////////////////////////////////////
//
//	main.c - Main functionality for pipeline simulator
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

int main(int argc, char *argv[]){

	int count = 0;		// argv index

	int debug = getDebug(argc, argv);
	char* input = getInputFile(argc, argv);
	char* output = getOutputFile(argc, argv);
	int mode = getMode(argc, argv);

	FILE* addressFile = openInputFile(input);	// open memory file to read from
	FILE* outputFile = openOutputFile(output);	// open file to output to

	////////////////////////////////////////////////////
	/*


//Heres just a little start of an idea, what do yall think? -Emily

//declare a 4kB of mem storage (array?)
//declare struct of registers 1-31


struct inst
{
    char type;
    int Opcode;
    int rs;
    int rt;
    int rd;
    int imm;
    bool halt;
};

//declare global clock counter, array[5], and circular buffer (5 entries)
//declare struct for currant instructions

    //input data from memory image into array?

    //enter loop that continues until halt

    while (halt != 1)
    {
        //increment counter
        //parse first instruction into binary
        //update array[5] with instruction
            //most first to IF, IF to ID, ID to EX, EX to MEM, MEM to WB
        //update circular buffer with instruction

        //check for RAW hazard, handel 
        
        //execute instruction in EX

        //save WB info for instruction in WB
    }





	*/
	/////////////////////////////////////////////////////

	closeFile(addressFile);	// close the input file 
	closeFile(outputFile);	// close the output file
 
	return 0;

}

// reads command line and returns debug flag if set
int getDebug(int index, char* commands[]){

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
