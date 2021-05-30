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
void printInstructionsByType(int arith, int logi, int mem, int ctl)
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

	return;
}

// Print final register and PC states
//
// reg[]: array of registers
// regChange[]: array with index corresponding to each register containing true if register has been changed
//	throughout the course of program execution, and false otherwise
// pc: program counter values
void printRegPcStates(int reg[], bool regChange[], int pc)
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
	

	return;
}

// Print final memory state
//
// mem[]: memory array
// memChange[]: array with index corresponding to each memory address containing true if address contents have
//	been changed throughout the course of program execution, and false otherwise
void printMemStates(int mem[], bool memChange[])
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
	return;
}

// Print number of data hazards, average stall per stall condition,
// 	and total execution time for non forwarding case
//
// nfHazards: number of data hazards for non-forwarding case
// totalStall: total stall from non-forwarding data hazards
// totalCycles: total execution time in clock cycles for non-forwarding case 
void printNoForwardingHazards(int nfHazards, int totalStall, int totalCycles)
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
	return;
}

// Print number of data hazards, average stall per stall condition,
// and total execution time for forwarding case
//
// fHazards: number of data hazards for forwarding case
// totalStall: total stall from forwarding data hazards
// totalCycles: total execution time in clock cycles for forwarding case 
void printForwardingHazards(int fHazards, int totalStall, int totalCycles)
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
	return;
}

// Print speedup achieved by implementing data forwarding
//
// nfCycles: total execution time in cycles for non-forwarding case
// fCycles: total execution time in cycles for forwarding case
void printSpeedupAchieved(int nfCycles, int fCycles)
{
	float speedup = ((float)nfCycles) / ((float)fCycles);
	printf("\n");
	printf("\nTotal speedup achieved with data forwarding: %f", speedup);
	printf("\n");
	return;
}
