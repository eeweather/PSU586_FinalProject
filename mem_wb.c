#include "main.h"

// MEM stage of pipeline
void memory_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[], bool memChange[])
{
	instructions[MEM] = instructions[EX];
	opcode_t opcode = instructions[MEM].opcode;
	uint32_t alu_temp = mips_status->alu_temp;
	uint8_t rt = instructions[MEM].rt;

	if (instructions[MEM].nop == false)
	{
		if (opcode == LDW)	// load instruction
		{
			mips_status->mem_reg = memory[alu_temp>>2];	// load value from memory into temporary register
		}
		else if (opcode == STW)	// store instruction
		{
			memory[alu_temp>>2] = registers[rt];	// store contents of register to memory address
			memChange[alu_temp>>2] = true;
		}
		else if (opcode == ADD || opcode == SUB || opcode == MUL || opcode == OR || opcode == AND || opcode == XOR)
		{
			mips_status->mem_reg = alu_temp;	// push alu value down the pipeline into mem-wb register
		}
		else if (opcode == ADDI	|| opcode == SUBI || opcode == MULI || opcode == ORI || opcode == ANDI || opcode == XORI)
		{
			mips_status->mem_reg = alu_temp;	// push alu value down the pipeline into mem-wb register
		}
	}
	if (mips_status->jump_flag == TRUE)
	{	// update program counter if jump taken
		mips_status->pc = alu_temp;
	}
	else
	{	// if no jump, update to new program counter value	
		mips_status->pc = mips_status->npc;
	}

	if (instructions[MEM].opcode == HALT)
	{
		mips_status->halt = true;
	}

	return;
}

// WB stage of pipeline
void writeback_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], bool regChange[])
{
	instructions[WB]=instructions[MEM];
	opcode_t opcode = instructions[WB].opcode;
	uint8_t rt = instructions[WB].rt;
	uint8_t rd = instructions[WB].rd;
	uint32_t mem_reg = mips_status->mem_reg;

	if (instructions[WB].nop == false)
	{
		if (opcode == LDW)
		{	// store value from memory to register if register is not R0
	
			if (rt != 0)
			{
				registers[rt] = mem_reg;
				regChange[rt] = true;
			}
		}

		else if (opcode == ADD || opcode == SUB || opcode == MUL || opcode == OR || opcode == AND || opcode == XOR)
		{	// write result to register unless register is R0
	
			if (rd != 0)
			{
				registers[rd] = mem_reg;
				regChange[rd] = true;
			}
		}

		else if (opcode == ADDI	|| opcode == SUBI || opcode == MULI || opcode == ORI || opcode == ANDI || opcode == XORI)
		{
			// i type instruction
			if(rt != 0)
			{
				registers[rt] = mem_reg;
				regChange[rt] = true;
			}
		}
	}

	else if (opcode == HALT)
	{	// halt the program
		mips_status->halt = true;
	}

	return;
}
