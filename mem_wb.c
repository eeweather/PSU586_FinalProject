#include "main.h"
#include "mem_wb.h"

// MEM stage of pipeline
void memory_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[])
{
	opcode_t opcode = instructions[MEM].opcode;
	uint32_t alu_temp = mips_status->alu_temp;

	if (opcode == LDW)	// load instruction
	{	// load value from memory into temporary register
		mips_status->mem_reg = memory[alu_temp>>2];
	}
	else if (opcode == STW)	// store instruction
	{	// store contents of register to memory address
		memory[alu_temp>>2] = registers[instructions[MEM].rt_i_type];
	}
	if (mips_status->jump_flag == TRUE)
	{	// update program counter if jump taken
		mips_status->pc = alu_temp;
	}
	else
	{	// if no jump, update to new program counter value	
		mips_status->pc = mips_status->npc;
	}

	instructions[WB]=instructions[MEM];

	return;
}

// WB stage of pipeline
void writeback_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[])
{
	opcode_t opcode = instructions[WB].opcode;
	uint32_t alu_temp = mips_status->alu_temp;

	if (opcode == LDW)
	{	// store value from memory to register if register is not R0
		if (instructions[WB].rt_i_type != 0)
		{
			registers[instructions[WB].rt_i_type] = mips_status->mem_reg;
		}
	}
	else if (opcode == ADD  // if R-type instruction
		|| opcode == ADDI
		|| opcode == SUB
		|| opcode == SUBI
		|| opcode == MUL
		|| opcode == MULI
		|| opcode == OR
		|| opcode == ORI
		|| opcode == AND
		|| opcode == ANDI
		|| opcode == XOR
		|| opcode == XORI)
	{	// write result to register unless register is R0
		if (instructions[WB].rd_r_type != 0)
		{
			registers[instructions[WB].rd_r_type] = alu_temp;
		}
	}
	else if (opcode == HALT)
	{	// halt the program
		mips_status->halt = TRUE;
	}

	return;
}
