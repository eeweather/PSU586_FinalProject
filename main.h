#ifndef _HEADER_H
#define _HEADER_H

// libraries and includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// definitions
#define FUNCTIONAL 	0	// simulator modes
#define TIMING		1
#define FORWARDING 	2

#define OFF 		0	// general definitions
#define ON		1

// function prototypes
int getDebug(int index, char* commands[]);
char* getInputFile(int index, char* commands[]);
char* getOutputFile(int index, char* commands[]);
int getMode(int index, char* commands[]);
FILE* openInputFile(const char* inputFile);
FILE* openOutputFile(const char* outputFile);
void closeFile(FILE* inputFile);

#endif
