#include "main.h"
#include "mem_wb.h"

// MEM stage of pipeline
void memory_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[])
{
	if (instructions[MEM].opcode == LDW)	// load instruction
	{	// load value from memory into temporary register
		mips_status->mem_reg = memory[(mips_status->alu_temp)>>2];
	}
	else if (instructions[MEM].opcode == STW)	// store instruction
	{	// store contents of register to memory address
		memory[(mips_status->alu_temp)>>2] = registers[instructions[MEM].rt];
	}
	else if (instructions[MEM].opcode == ADD  // if R-type instruction
		|| instructions[MEM].opcode == ADDI
		|| instructions[MEM].opcode == SUB
		|| instructions[MEM].opcode == SUBI
		|| instructions[MEM].opcode == MUL
		|| instructions[MEM].opcode == MULI
		|| instructions[MEM].opcode == OR
		|| instructions[MEM].opcode == ORI
		|| instructions[MEM].opcode == AND
		|| instructions[MEM].opcode == ANDI
		|| instructions[MEM].opcode == XOR
		|| instructions[MEM].opcode == XORI)
	{	// write result to register unless register is R0
		if (instructions[MEM].rd != 0)
		{
			registers[instructions[MEM].rd] = mips_status->alu_temp;
		}
	}
	if (mips_status->jump_flag == TRUE)
	{	// update program counter if jump taken
		mips_status->pc = mips_status->alu_temp;
	}
	else
	{	// if no jump, update to new program counter value	
		mips_status->pc = mips_status->npc;
	}

	return;
}

// WB stage of pipeline
void writeback_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[])
{
	if (instructions[WB].opcode == LDW)
	{	// store value from memory to register if register is not R0
		if (instructions[WB].rt != 0)
		{
			registers[instructions[WB].rt] = mips_status->mem_reg;
		}
	}
	else if (instructions[WB].opcode == HALT)
	{	// halt the program
		mips_status->halt = TRUE;
	}

	return;
}
