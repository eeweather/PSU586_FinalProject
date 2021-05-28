#include "main.h"
#include "mem_wb.h"

// MEM stage of pipeline
void memory_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[], bool memChange[])
{
	opcode_t opcode = instructions[MEM].opcode;
	uint32_t alu_temp = mips_status->alu_temp;

	if (opcode == LDW)	// load instruction
	{	// load value from memory into temporary register
		mips_status->mem_reg = memory[alu_temp>>2];
	}
	else if (opcode == STW)	// store instruction
	{	// store contents of register to memory address
		memory[alu_temp>>2] = registers[instructions[MEM].rt];
		memChange[alu_temp>>2] = true;
	}
	if (mips_status->jump_flag == TRUE)
	{	// update program counter if jump taken
		//mips_status->pc = alu_temp;
	}
	else
	{	// if no jump, update to new program counter value	
		//mips_status->pc = mips_status->npc;
	}

	instructions[WB]=instructions[MEM];

	return;
}

// WB stage of pipeline
void writeback_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], bool regChange[])
{
	opcode_t opcode = instructions[WB].opcode;
	uint32_t alu_temp = mips_status->alu_temp;

	if (opcode == LDW)
	{	// store value from memory to register if register is not R0

		if (instructions[WB].rt != 0)
		{
			registers[instructions[WB].rt] = mips_status->mem_reg;
			regChange[instructions[WB].rt] = true;
		}
	}
	else if (opcode == ADD  // if R-type instruction
		|| opcode == SUB
		|| opcode == MUL
		|| opcode == OR
		|| opcode == AND
		|| opcode == XOR)
	{	// write result to register unless register is R0

		if (instructions[WB].rd != 0)
		{
			// printf("what is in WB %c\n",instructions[WB].type);
			// printf("What register to write to: %d\n",instructions[WB].rd);
			
			registers[instructions[WB].rd] = alu_temp;
			regChange[instructions[WB].rd] = true;
			
		}
	}
	else if (opcode == ADDI
		|| opcode == SUBI
		|| opcode == MULI
		|| opcode == ORI
		|| opcode == ANDI
		|| opcode == XORI)
	{
		// i type instruction
		if(instructions[WB].rt !=0){
			
			// printf("what is in WB %c\n",instructions[WB].type);
			// printf("What register to write to: %d\n",instructions[WB].rd);
			
			registers[instructions[WB].rt] = alu_temp;
			regChange[instructions[WB].rt] = true;
			
		}
	}
	else if (opcode == HALT)
	{	// halt the program
		mips_status->halt = TRUE;
	}

	return;
}
