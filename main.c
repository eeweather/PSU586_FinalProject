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

	int debug = OFF;  	// keep track of simulator mode and whether debug is active
	int mode = FUNCTIONAL;	// default to functional simulation mode
	int count = 0;		// argv index

	char *input = "input.txt";   // default input file name
	char *output = "output.txt"; // default output file name

	if (argc > 1){

		// check for debug argument
		for (count = 1; count < argc; count++){

			if (strcmp(argv[count], "-d") == 0){
				debug = ON;
			}
		}

		// check for input file argument
		for (count = 1; count < argc; count++){

			if (strcmp(argv[count], "-i") == 0){

				//check if the second argument is empty
				if(argv[count+1] == NULL){
					printf("Error: Argument cannot be empty\n"); 
					exit(1);
				}

				input = argv[count + 1];
			}
		}

		// check for output file argument
		for (count = 1; count < argc; count++){

			if (strcmp(argv[count], "-o") == 0){

				//check if the second argument is empty
				if(argv[count+1] == NULL){
					printf("Error: Argument cannot be empty\n");
					exit(1);
				}

				output = argv[count + 1];
			} 
		}	

		// check for mode argument
		for (count = 1; count < argc; count++){

			if (strcmp(argv[count], "-m") == 0){

				//check if the second argument is empty
				if(argv[count+1] == NULL){
					printf("Error: Argument cannot be empty\n");
					exit(1);
				}

				else if(strcmp(argv[count+1], "0") == 0){
					mode = FUNCTIONAL;
					printf("\nMode: Functional simulation only\n");
				}

				else if(strcmp(argv[count+1], "1") == 0){
					mode = TIMING;
					printf("\nMode: Simulation with timing\n");
				}

				else if(strcmp(argv[count+1], "2") == 0){
					mode = FORWARDING;
					printf("\nMode: Simulation with timing and data forwarding\n");
				}

				else{
					printf("Error: Invalid mode argument\n");
					exit(1);
				}

			} // end if
	
		} // end for

	} // end if
	
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
