#ifndef _FORMATTING_H
#define _FORMATTING_H

#include "main.h"

// Function Prototypes
void printInstructionsByType(int arith, int logi, int mem, int ctl, FILE* outputFile);
void printRegPcStates(int reg[], bool regChange[], int pc, FILE* outputFile);
void printMemStates(int mem[], bool memChange[], FILE* outputFile);
void printNoForwardingHazards(int nfHazards, int totalStall, int totalCycles, FILE* outputFile);
void printForwardingHazards(int fHazards, int totalStall, int totalCycles, FILE* outputFile);
void printSpeedupAchieved(int nfCycles, int fCycles, FILE* outputFile);

#endif
