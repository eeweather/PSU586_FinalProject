#ifndef _FORMATTING_H
#define _FORMATTING_H

#include "main.h"

// Function Prototypes
void printInstructionsByType(int arith, int logi, int mem, int ctl, FILE* outputFile);
void printRegPcStates(int reg[], bool regChange[], int pc, FILE* outputFile);
void printMemStates(int mem[], bool memChange[], FILE* outputFile);
void printHazards(int hazards, int totalStall, int totalCycles, FILE* outputFile);

#endif
