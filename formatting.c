///////////////////////////////////////////////////////////
//
//	formatting.c - output formatting for simulator
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "formatting.h"

// Print number of instructions by type
//
// arith: number of arithmetic instructions
// logi: number of logical instructions
// mem: number of memory instructions
// ctl: number of control instructions
void printInstructionsByType(int arith, int logi, int mem, int ctl, FILE* outputFile)
{
	int total = arith + logi + mem + ctl;

	printf("\n");
	printf("\nInstruction counts");
	printf("\n");
	printf("\nTotal number of instructions: %d", total);
	printf("\nArithmetic instructions: %d", arith);
	printf("\nLogical instructions: %d", logi);
	printf("\nMemory access instructions: %d", mem);
	printf("\nControl transfer instructions: %d", ctl);
	printf("\n");

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nInstruction counts");
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal number of instructions: %d", total);
	fprintf(outputFile, "\nArithmetic instructions: %d", arith);
	fprintf(outputFile, "\nLogical instructions: %d", logi);
	fprintf(outputFile, "\nMemory access instructions: %d", mem);
	fprintf(outputFile, "\nControl transfer instructions: %d", ctl);
	fprintf(outputFile, "\n");

	return;
}

// Print final register and PC states
//
// reg[]: array of registers
// regChange[]: array with index corresponding to each register containing true if register has been changed
//	throughout the course of program execution, and false otherwise
// pc: program counter values
void printRegPcStates(int reg[], bool regChange[], int pc, FILE* outputFile)
{
	printf("\n");
	printf("\nFinal register state");
	printf("\n");
	printf("\nProgram Counter: %d", pc);
	for (int i = 0; i < 32; i++)
	{
		if (regChange[i])
		{
			printf("\nR%d: %d", i, reg[i]);
		}
	}
	printf("\n");
	
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nFinal register state");
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nProgram Counter: %d", pc);
	for (int i = 0; i < 32; i++)
	{
		if (regChange[i])
		{
			fprintf(outputFile, "\nR%d: %d", i, reg[i]);
		}
	}
	fprintf(outputFile, "\n");

	return;
}

// Print final memory state
//
// mem[]: memory array
// memChange[]: array with index corresponding to each memory address containing true if address contents have
//	been changed throughout the course of program execution, and false otherwise
void printMemStates(int mem[], bool memChange[], FILE* outputFile)
{
	printf("\n");
	printf("\nFinal memory state");
	printf("\n");
	for (int i = 0; i < 1024; i++)
	{
		if (memChange[i])
		{
			printf("\nAddress: %d, contents %d", i, mem[i]);
		}
	}
	printf("\n");

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nFinal memory state");
	fprintf(outputFile, "\n");
	for (int i = 0; i < 1024; i++)
	{
		if (memChange[i])
		{
			fprintf(outputFile, "\nAddress: %d, contents %d", i, mem[i]);
		}
	}
	fprintf(outputFile, "\n");

	return;
}

// Print number of data hazards, average stall per stall condition,
// 	and total execution time for non forwarding case
//
// nfHazards: number of data hazards for non-forwarding case
// totalStall: total stall from non-forwarding data hazards
// totalCycles: total execution time in clock cycles for non-forwarding case 
void printNoForwardingHazards(int nfHazards, int totalStall, int totalCycles, FILE* outputFile)
{
	float avgStall = ((float)totalStall) / ((float)nfHazards);

	printf("\n");
	printf("\nNo forwarding case");
	printf("\n");
	printf("\nTotal hazards: %d", nfHazards);
	printf("\nTotal stalls: %d", totalStall);
	printf("\nAverage stall per hazard: %f", avgStall);
	printf("\n");
	printf("\nTotal execution time: %d", totalCycles);
	printf("\n");

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nNo forwarding case");
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal hazards: %d", nfHazards);
	fprintf(outputFile, "\nTotal stalls: %d", totalStall);
	fprintf(outputFile, "\nAverage stall per hazard: %f", avgStall);
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal execution time: %d", totalCycles);
	fprintf(outputFile, "\n");

	return;
}

// Print number of data hazards, average stall per stall condition,
// and total execution time for forwarding case
//
// fHazards: number of data hazards for forwarding case
// totalStall: total stall from forwarding data hazards
// totalCycles: total execution time in clock cycles for forwarding case 
void printForwardingHazards(int fHazards, int totalStall, int totalCycles, FILE* outputFile)
{
	float avgStall = ((float)totalStall) / ((float)fHazards);

	printf("\n");
	printf("\nForwarding case");
	printf("\n");
	printf("\nTotal hazards: %d", fHazards);
	printf("\nTotal stalls: %d", totalStall);
	printf("\nAverage stall per hazard: %f", avgStall);
	printf("\n");
	printf("\nTotal execution time: %d", totalCycles);
	printf("\n");

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nForwarding case");
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal hazards: %d", fHazards);
	fprintf(outputFile, "\nTotal stalls: %d", totalStall);
	fprintf(outputFile, "\nAverage stall per hazard: %f", avgStall);
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal execution time: %d", totalCycles);
	fprintf(outputFile, "\n");

	return;
}

// Print speedup achieved by implementing data forwarding
//
// nfCycles: total execution time in cycles for non-forwarding case
// fCycles: total execution time in cycles for forwarding case
void printSpeedupAchieved(int nfCycles, int fCycles, FILE* outputFile)
{
	float speedup = ((float)nfCycles) / ((float)fCycles);

	printf("\n");
	printf("\nTotal speedup achieved with data forwarding: %f", speedup);
	printf("\n");

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\nTotal speedup achieved with data forwarding: %f", speedup);
	fprintf(outputFile, "\n");

	return;
}
