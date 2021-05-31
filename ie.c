///////////////////////////////////////////////////////////
//
//	ie.c - Instruction Execute phase of pipeline
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

void execution_stage(inst_t instructions[], mips_status_t *mips_status, int32_t registers[], forward_stage_t* forward_stage_flag, forward_reg_t* forward_reg_flag)
{

/*
    switch (*forward_stage_flag){
        case (NO_FWDH):
            printf("No forwarding hazard\n");
            break;
        case (EX_MEM):
            printf("Forwarding hazard, need data from EX stage\n");
            break;
        case (MEM_WB):
            printf("Forwarding hazard, need data from MEM stage\n");
            break;
    }
    switch (*forward_reg_flag){
        case (RS):
            printf("Forwarding hazard for RS reg\n");
            break;
        case (RT):
            printf("Forwarding hazard for RT reg\n");
            break;
    }
*/

    instructions[EX] = instructions[ID];
    if(instructions[EX].nop == true){
    }
    else{

        inst_t current_int = instructions[EX];

        int32_t rs_value = registers[current_int.rs];
        int32_t rt_value = registers[current_int.rt];

        if(*forward_stage_flag == NO_FWDH){
            //do nothing
        }
        else if(*forward_stage_flag == EX_MEM){
            if(*forward_reg_flag == RS){
                rs_value = mips_status->alu_temp;
            }
            else if(*forward_reg_flag == RT){
                rt_value = mips_status->alu_temp;
            }
            else{
                printf("Error with EX forwarding to incorrect register, not RS or RT.\n");
            }
        }else if(*forward_stage_flag == MEM_WB){
            if(*forward_reg_flag == RS){
                rs_value = mips_status->prior_alu_temp;
            }
            else if(*forward_reg_flag == RT){
                rt_value = mips_status->prior_alu_temp;
            }
            else{
                printf("Error with MEM forwarding to incorrect register, not RS or RT.\n");
            }
        }
        *forward_stage_flag = NO_FWDH; //resolve this back to no forwarding hazard until we get a true hazard error again

        //printf("current Opcode: %x-------------------------------\n",current_int.opcode);
        
        mips_status->prior_alu_temp = mips_status->alu_temp; //copy the alu_temp to have for MEM forwarding (previous instruction info)
        switch (current_int.opcode)
        {
        //ADD Rd Rs Rt (Add the contents of registers Rs and Rt, transfer the result
        //to register Rd). Opcode: 000000
        case (ADD):
            mips_status->alu_temp = rs_value + rt_value;
            //printf("Value of Temp register in ADD: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            //printf("ADD arith count is %d\n", mips_status_t->count_arith);
            break;

        //ADDI Rt Rs Imm (Add the contents of register Rs to the immediate value “Imm”,
        //transfer the result to register Rt). Opcode: 000001
        case (ADDI):
            mips_status->alu_temp = rs_value + current_int.imm;
            //printf("Value of Temp register in ADDI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            //printf("ADDI arith count is %d\n", mips_status_t->count_arith);
            break;

        //SUB Rd Rs Rt (Subtract the contents of register Rt from Rs, transfer the result to
        //register Rd). Opcode: 000010
        case (SUB):
            mips_status->alu_temp = rs_value - rt_value;
            //printf("Value of Temp register in SUB: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            //printf("SUB arith count is %d\n", mips_status_t->count_arith);
            break;

        //SUBI Rt Rs Imm (Subtract the immediate value “Imm” from the contents of register Rs,
        //transfer the result to register Rt). Opcode: 000011
        case (SUBI):
            mips_status->alu_temp = rs_value - current_int.imm;
            //printf("Value of Temp register in SUBI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            break;

        //MUL Rd Rs Rt (Multiply the contents of registers Rs and Rt, transfer the result to
        //register Rd). Opcode: 000100
        case (MUL):
            mips_status->alu_temp = rs_value * rt_value;
            //printf("Value of Temp register in MUL: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            break;

        //MULI Rt Rs Imm (Multiply the contents of registers Rs with the immediate value
        //“Imm”, transfer the result to register Rt). Opcode: 000101
        case (MULI):
            mips_status->alu_temp = rs_value * current_int.imm;
            //printf("Value of Temp register in MULI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_arith++;
            mips_status->count_total++;
            break;

        //OR Rd Rs Rt (Take a bitwise OR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 000110
        case (OR):
            mips_status->alu_temp = rs_value | rt_value;
            //printf("Value of Temp register in logical OR: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //ORI Rt Rs Imm (Take a bitwise OR of the contents of registers Rs and the immediate
        //value “Imm”, transfer the result to register Rt). Opcode: 000111
        case (ORI):
            mips_status->alu_temp = rs_value | current_int.imm;
            //printf("Value of Temp register in logical ORI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //AND Rd Rs Rt (Take a bitwise AND of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001000
        case (AND):
            mips_status->alu_temp = rs_value & rt_value;
            //printf("Value of Temp register in logical AND: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //ANDI Rt Rs Imm (Take a bitwise AND of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001001
        case (ANDI):
            mips_status->alu_temp = rs_value & current_int.imm;
            //printf("Value of Temp register in logical ANDI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //XOR Rd Rs Rt (Take a bitwise XOR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001010
        case (XOR):
            mips_status->alu_temp = rs_value ^ rt_value;
            //printf("Value of Temp register in logical XOR: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //XORI Rt Rs Imm (Take a bitwise XOR of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001011
        case (XORI):
            mips_status->alu_temp = rs_value ^ current_int.imm;
            //printf("Value of Temp register in logical XORI: %d\n", mips_status_t->alu_temp);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_logic++;
            mips_status->count_total++;
            break;

        //LDW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, load the contents (32-bits) of the memory location at address
        //“A” into register Rt). Opcode: 001100
        case (LDW):
            mips_status->mem_reg = rs_value + current_int.imm;
            // printf("Value of the register rs in r type:%d\n", rs_value);
            // printf("Value of the register rs in i type: %d\n", current_int.rs);
            // printf("Value of register type: %d\n", current_int.rs);
            // printf("Value of the register r1 in array:%d\n", registers[1]);
            // printf("Final value of the mem_reg variable: %d\n", mips_status_t->mem_reg);
            mips_status->mem_reg = (mips_status->mem_reg / 4);
            //printf("Final value of the mem_reg variable after dividing it by 4: %d\n", mips_status_t->mem_reg);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_total++;
            mips_status->count_memory_access++;
            //printf("MemAccess count is %d\n", mips_status_t->count_memory_access);
            break;

        //STW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, store the contents of register Rt (32-bits) at the memory
        //address “A”). Opcode: 001101
        case (STW):
            mips_status->mem_reg = rs_value + current_int.imm;
            mips_status->mem_reg = (mips_status->mem_reg / 4);
            // mips_status_t->pc = mips_status_t->pc + 1;
            // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status->count_total++;
            mips_status->count_memory_access++;
            break;

        //BZ Rs x (If the contents of register Rs are zero, then branch to the “x”th instruction from
        //the current instruction). Opcode: 001110
        case (BZ):
            if ((rs_value) == 0)
            {
                mips_status->zero_flag = true;
                //printf("Conditional BZ  Branch is taken\n");
                mips_status->pc = (current_int.imm + mips_status->pc);
                mips_status->temp_pc = (mips_status->pc << 2);
                mips_status->count_control_flow++;
                mips_status->count_total++;
            }
            else
            {
                //printf("Branch BZ is not taken\n");
                // mips_status_t->pc = mips_status_t->pc + 1;
                // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
                mips_status->count_control_flow++;
                mips_status->count_total++;
            }
            break;

        //BEQ Rs Rt x (Compare the contents of registers Rs and Rt. If they are equal, then
        //branch to the “x”th instruction from the current instruction). Opcode: 001111
        case (BEQ):
            if ((rt_value) == (rs_value))
            {
                mips_status->zero_flag = true;
                //printf("Conditional BEQ Branch is taken\n");
                //printf("before BEQ math, imm: %d and pc: %d\n", current_int.imm,(int) mips_status->pc);
                instructions[IF].binary = 0; //maybe dont need
                instructions[IF].opcode = NON; //maybe dont need
                if(current_int.imm >= 8){
                    instructions[ID].binary = 0; //maybe dont need
                    instructions[ID].opcode = NON; //maybe dont need
                }
                mips_status->pc = (current_int.imm -4 + mips_status->pc);
                //printf("pc after adding imm in BEQ: %d\n", (int) mips_status->pc);
                mips_status->temp_pc = (mips_status->pc << 2);
                mips_status->count_control_flow++;
                mips_status->count_total++;
                //printf("Control flow count is %d\n", mips_status_t->count_control_flow);
            }
            else
            {
                //printf("Branch BEQ is not taken\n");
                // mips_status_t->pc = mips_status_t->pc + 1;
                // mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
                mips_status->count_control_flow++;
                mips_status->count_total++;
                //printf("Control flow count is %d\n", mips_status_t->count_control_flow);
            }
            break;

        //JR Rs (Load the PC [program counter] with the contents of register Rs. Jump to the new
        //PC). Opcode: 010000
        case (JR):
            mips_status->jump_flag = true;
            //printf(" Unconditional Branch is taken\n");
            // mips_status_t->temp_pc = rs_value;
            // mips_status_t->pc = (mips_status_t->temp_pc / 4);
            mips_status->count_control_flow++;
            mips_status->count_total++;
            break;
            
        //HALT (Stop executing the program). Opcode: 010001
        case (HALT):
            //printf(" Halt case\n");
            mips_status->count_control_flow++;
            mips_status->count_total++;
            break;
            
        case (NON):
            // Not an opcode, initialization as none
            break;
            
        // Simulator shouldn't reach the default state
        default:
            printf("Default Case reached in Execute Stage.  Something isn't working.\n");
            break;
        }
        //printf("current count: %d-------------------------------\n",mips_status->count_total);
        printf("current pc: %d\n",mips_status->pc);
        instructions[EX] = current_int;
    }
    return;
}
