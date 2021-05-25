#ifndef _FORMATTING_H
#define _FORMATTING_H

#include "main.h"

// Function Prototypes
void printInstructionsByType(int arith, int logi, int mem, int ctl);
void printRegPcStates(int reg[], bool regChange[], int pc);
void printMemStates(int mem[], bool memChange[]);
void printNoForwardingHazards(int nfHazards, int totalStall, int totalCycles);
void printForwardingHazards(int fHazards, int totalStall, int totalCycles);
void printSpeedupAchieved(int nfCycles, int fCycles);

#endif
