///////////////////////////////////////////////////////////
//
//	ie.c - Instruction Execute phase of pipeline
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

void execution_stage(inst_t instructions[], struct mips_status *mips_status_t,int32_t registers[])
{

    instructions[EX] = instructions[ID];
    if(instructions[EX].nop == true){
    }
    else{
        inst_t current_int = instructions[EX];

        //printf("current Opcode: %x-------------------------------\n",current_int.opcode);
        
        switch (current_int.opcode)
        {
        //ADD Rd Rs Rt (Add the contents of registers Rs and Rt, transfer the result
        //to register Rd). Opcode: 000000
        case (ADD):
            mips_status_t->alu_temp = registers[current_int.rs] + registers[current_int.rt];
            //printf("Value of Temp register in ADD: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            //printf("ADD arith count is %d\n", mips_status_t->count_arith);
            break;

        //ADDI Rt Rs Imm (Add the contents of register Rs to the immediate value “Imm”,
        //transfer the result to register Rt). Opcode: 000001
        case (ADDI):
            mips_status_t->alu_temp = registers[current_int.rs] + current_int.imm;
            //printf("Value of Temp register in ADDI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            //printf("ADDI arith count is %d\n", mips_status_t->count_arith);
            break;

        //SUB Rd Rs Rt (Subtract the contents of register Rt from Rs, transfer the result to
        //register Rd). Opcode: 000010
        case (SUB):
            mips_status_t->alu_temp = registers[current_int.rs] - registers[current_int.rt];
            //printf("Value of Temp register in SUB: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            //printf("SUB arith count is %d\n", mips_status_t->count_arith);
            break;

        //SUBI Rt Rs Imm (Subtract the immediate value “Imm” from the contents of register Rs,
        //transfer the result to register Rt). Opcode: 000011
        case (SUBI):
            mips_status_t->alu_temp = registers[current_int.rs] - current_int.imm;
            //printf("Value of Temp register in SUBI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            break;

        //MUL Rd Rs Rt (Multiply the contents of registers Rs and Rt, transfer the result to
        //register Rd). Opcode: 000100
        case (MUL):
            mips_status_t->alu_temp = registers[current_int.rs] * registers[current_int.rt];
            //printf("Value of Temp register in MUL: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            break;

        //MULI Rt Rs Imm (Multiply the contents of registers Rs with the immediate value
        //“Imm”, transfer the result to register Rt). Opcode: 000101
        case (MULI):
            mips_status_t->alu_temp = registers[current_int.rs] * current_int.imm;
            //printf("Value of Temp register in MULI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_arith++;
            mips_status_t->count_total++;
            break;

        //OR Rd Rs Rt (Take a bitwise OR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 000110
        case (OR):
            mips_status_t->alu_temp = registers[current_int.rs] | registers[current_int.rt];
            //printf("Value of Temp register in logical OR: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //ORI Rt Rs Imm (Take a bitwise OR of the contents of registers Rs and the immediate
        //value “Imm”, transfer the result to register Rt). Opcode: 000111
        case (ORI):
            mips_status_t->alu_temp = registers[current_int.rs] | current_int.imm;
            //printf("Value of Temp register in logical ORI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //AND Rd Rs Rt (Take a bitwise AND of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001000
        case (AND):
            mips_status_t->alu_temp = registers[current_int.rs] & registers[current_int.rt];
            //printf("Value of Temp register in logical AND: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //ANDI Rt Rs Imm (Take a bitwise AND of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001001
        case (ANDI):
            mips_status_t->alu_temp = registers[current_int.rs] & current_int.imm;
            //printf("Value of Temp register in logical ANDI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //XOR Rd Rs Rt (Take a bitwise XOR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001010
        case (XOR):
            mips_status_t->alu_temp = registers[current_int.rs] ^ registers[current_int.rt];
            //printf("Value of Temp register in logical XOR: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //XORI Rt Rs Imm (Take a bitwise XOR of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001011
        case (XORI):
            mips_status_t->alu_temp = registers[current_int.rs] ^ current_int.imm;
            //printf("Value of Temp register in logical XORI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_logic++;
            mips_status_t->count_total++;
            break;

        //LDW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, load the contents (32-bits) of the memory location at address
        //“A” into register Rt). Opcode: 001100
        case (LDW):
            mips_status_t->mem_reg = registers[current_int.rs] + current_int.imm;
            // printf("Value of the register rs in r type:%d\n", registers[current_int.rs]);
            // printf("Value of the register rs in i type: %d\n", current_int.rs);
            // printf("Value of register type: %d\n", current_int.rs);
            // printf("Value of the register r1 in array:%d\n", registers[1]);
            // printf("Final value of the mem_reg variable: %d\n", mips_status_t->mem_reg);
            mips_status_t->mem_reg = (mips_status_t->mem_reg / 4);
            //printf("Final value of the mem_reg variable after dividing it by 4: %d\n", mips_status_t->mem_reg);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_total++;
            mips_status_t->count_memory_access++;
            //printf("MemAccess count is %d\n", mips_status_t->count_memory_access);
            break;

        //STW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, store the contents of register Rt (32-bits) at the memory
        //address “A”). Opcode: 001101
        case (STW):
            mips_status_t->mem_reg = registers[current_int.rs] + current_int.imm;
            mips_status_t->mem_reg = (mips_status_t->mem_reg / 4);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_total++;
            mips_status_t->count_memory_access++;
            break;

        //BZ Rs x (If the contents of register Rs are zero, then branch to the “x”th instruction from
        //the current instruction). Opcode: 001110
        case (BZ):
            if ((registers[current_int.rs]) == 0)
            {
                mips_status_t->zero_flag = true;
                //printf("Conditional BZ  Branch is taken\n");
                mips_status_t->pc = (current_int.imm + mips_status_t->pc);
                mips_status_t->temp_pc = (mips_status_t->pc << 2);
                mips_status_t->count_control_flow++;
                mips_status_t->count_total++;
            }
            else
            {
                //printf("Branch BZ is not taken\n");
                // mips_status_t->pc = mips_status_t->pc + 1;
                // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
                mips_status_t->count_control_flow++;
                mips_status_t->count_total++;
            }
            break;

        //BEQ Rs Rt x (Compare the contents of registers Rs and Rt. If they are equal, then
        //branch to the “x”th instruction from the current instruction). Opcode: 001111
        case (BEQ):
            if ((registers[current_int.rt]) == (registers[current_int.rs]))
            {
                mips_status_t->zero_flag = true;
                //printf("Conditional BEQ Branch is taken\n");
                mips_status_t->pc = (current_int.imm + mips_status_t->pc);
                mips_status_t->temp_pc = (mips_status_t->pc << 2);
                mips_status_t->count_control_flow++;
                mips_status_t->count_total++;
                //printf("Control flow count is %d\n", mips_status_t->count_control_flow);
            }
            else
            {
                //printf("Branch BEQ is not taken\n");
                // mips_status_t->pc = mips_status_t->pc + 1;
                // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
                mips_status_t->count_control_flow++;
                mips_status_t->count_total++;
                //printf("Control flow count is %d\n", mips_status_t->count_control_flow);
            }
            break;

        //JR Rs (Load the PC [program counter] with the contents of register Rs. Jump to the new
        //PC). Opcode: 010000
        case (JR):
            mips_status_t->jump_flag = true;
            //printf(" Unconditional Branch is taken\n");
            // mips_status_t->temp_pc = registers[current_int.rs];
            // mips_status_t->pc = (mips_status_t->temp_pc / 4);
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
            break;
            
        //HALT (Stop executing the program). Opcode: 010001
        case (HALT):
            //printf(" Halt case\n");
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
            break;

        // Simulator shouldn't reach the default state
        default:
            printf("Default Case reached in Execute Stage.  Something isn't working.\n");
            break;
        }
        printf("current pc: %d\n",mips_status_t->pc);
        instructions[EX] = current_int;
    }
    return;
}
