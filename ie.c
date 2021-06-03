///////////////////////////////////////////////////////////
//
//	ie.c - Instruction Execute phase of pipeline
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

void execution_stage (inst_t instructions[], struct mips_status *mips_status, int32_t registers[], forward_stage_t* forward_stage_flag, forward_reg_t* forward_reg_flag, mem_stage_values_t mem_values[], ex_stage_values_t ex_values[])
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
    //check for nop, and skip execution logic if true

    if(instructions[EX].nop == true){
        //do nopthing
    }
    //check for forward flags
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
                printf("assigning alu_temp to rs_value: %d\n",rs_value);
            }
            else if(*forward_reg_flag == RT){
                
                rt_value = mips_status->alu_temp;
                printf("assigning alu_temp to rt_value: %d\n",rt_value);
            }
            else{
                printf("Error with EX forwarding to incorrect register, not RS or RT.\n");
            }
        }else if(*forward_stage_flag == MEM_WB){
            if(*forward_reg_flag == RS){
                rs_value = mips_status->mem_reg;
                printf("assigning mem_reg to rs_value: %d\n",rs_value);
            }
            else if(*forward_reg_flag == RT){
                rt_value = mips_status->mem_reg;
                printf("assigning mem_reg to rt_value: %d\n",rt_value);

            }
            else{
                printf("Error with MEM forwarding to incorrect register, not RS or RT.\n");
            }
        }
        *forward_stage_flag = NO_FWDH; //resolve this back to no forwarding hazard until we get a true hazard error again

        printf("current Opcode: %x-------------------------------\n",current_int.opcode);
        //printf("current rd: %x-------------------------------\n",current_int.rd);
        
        mips_status->prior_alu_temp = mips_status->alu_temp; //copy the alu_temp to have for MEM forwarding (previous instruction info)
        switch (current_int.opcode)
        {
        //ADD Rd Rs Rt (Add the contents of registers Rs and Rt, transfer the result
        //to register Rd). Opcode: 000000
        case (ADD):
            if(current_int.rd ==0){
                break;  //if destination is zero, skip case
            }
            mips_status->alu_temp = rs_value + rt_value;
            printf("ADD, alu temp: %d rs: %d rt: %d\n", mips_status->alu_temp, rs_value, rt_value);
            mips_status->count_arith++;
            break;

        //ADDI Rt Rs Imm (Add the contents of register Rs to the immediate value “Imm”,
        //transfer the result to register Rt). Opcode: 000001
        case (ADDI):
            mips_status->alu_temp = rs_value + current_int.imm;
            printf("ADDI, alu temp: %d rs: %d imm: %d\n", mips_status->alu_temp, rs_value, current_int.imm);
            mips_status->count_arith++;
            break;

        //SUB Rd Rs Rt (Subtract the contents of register Rt from Rs, transfer the result to
        //register Rd). Opcode: 000010
        case (SUB):
            mips_status->alu_temp = rs_value - rt_value;
            printf("SUB, alu temp: %d rs: %d rt: %d\n", mips_status->alu_temp, rs_value, rt_value);
            mips_status->count_arith++;
            break;

        //SUBI Rt Rs Imm (Subtract the immediate value “Imm” from the contents of register Rs,
        //transfer the result to register Rt). Opcode: 000011
        case (SUBI):
            mips_status->alu_temp = rs_value - current_int.imm;
            mips_status->count_arith++;
            break;

        //MUL Rd Rs Rt (Multiply the contents of registers Rs and Rt, transfer the result to
        //register Rd). Opcode: 000100
        case (MUL):
            mips_status->alu_temp = rs_value * rt_value;
            mips_status->count_arith++;
            break;

        //MULI Rt Rs Imm (Multiply the contents of registers Rs with the immediate value
        //“Imm”, transfer the result to register Rt). Opcode: 000101
        case (MULI):
            mips_status->alu_temp = rs_value * current_int.imm;
            mips_status->count_arith++;
            break;

        //OR Rd Rs Rt (Take a bitwise OR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 000110
        case (OR):
            mips_status->alu_temp = rs_value | rt_value;
            mips_status->count_logic++;
            break;

        //ORI Rt Rs Imm (Take a bitwise OR of the contents of registers Rs and the immediate
        //value “Imm”, transfer the result to register Rt). Opcode: 000111
        case (ORI):
            mips_status->alu_temp = rs_value | current_int.imm;
            mips_status->count_logic++;
            break;

        //AND Rd Rs Rt (Take a bitwise AND of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001000
        case (AND):
            mips_status->alu_temp = rs_value & rt_value;
            mips_status->count_logic++;
            break;

        //ANDI Rt Rs Imm (Take a bitwise AND of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001001
        case (ANDI):
            mips_status->alu_temp = rs_value & current_int.imm;
            printf("ANDI, alu temp: %d rs: %d imm: %d\n", mips_status->alu_temp, rs_value, current_int.imm);
            mips_status->count_logic++;
            break;

        //XOR Rd Rs Rt (Take a bitwise XOR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001010
        case (XOR):
            mips_status->alu_temp = rs_value ^ rt_value;
            mips_status->count_logic++;
            break;

        //XORI Rt Rs Imm (Take a bitwise XOR of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001011
        case (XORI):
            mips_status->alu_temp = rs_value ^ current_int.imm;
            mips_status->count_logic++;
            break;

        //LDW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, load the contents (32-bits) of the memory location at address
        //“A” into register Rt). Opcode: 001100
        case (LDW):
            mips_status->alu_temp = rs_value + current_int.imm;
            printf("LDW, alu temp: %d rs: %d imm: %d\n", mips_status->alu_temp, rs_value, current_int.imm);
            mips_status->count_memory_access++;
            break;

        //STW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, store the contents of register Rt (32-bits) at the memory
        //address “A”). Opcode: 001101
        case (STW):
            mips_status->alu_temp = rs_value + current_int.imm;
            printf("STW, alu temp: %d rs: %d imm: %d\n", mips_status->alu_temp, rs_value, current_int.imm);
            mips_status->count_memory_access++;
            break;

        //BZ Rs x (If the contents of register Rs are zero, then branch to the “x”th instruction from
        //the current instruction). Opcode: 001110
        case (BZ):
            if ((rs_value) == 0)
            {
                mips_status->jump_flag = true;
                mips_status->flushcount=2;
                //instructions[IF].nop = TRUE;
                //instructions[ID].nop = TRUE;
                mips_status->alu_temp = (((current_int.imm<<2)-4) + mips_status->pc);
                printf("BZ taken, alu temp: %d, imm: %d, imm<<2: %d, pc: %d\n", mips_status->alu_temp, current_int.imm, (current_int.imm<<2), mips_status->pc);
                mips_status->count_control_flow++;
            }
            else
            {
                printf("Branch BZ is not taken\n");
                mips_status->count_control_flow++;
            }
            break;

        //BEQ Rs Rt x (Compare the contents of registers Rs and Rt. If they are equal, then
        //branch to the “x”th instruction from the current instruction). Opcode: 001111
        case (BEQ):
            if ((rt_value) == (rs_value))
            {
                mips_status->jump_flag = true;
                mips_status->flushcount=2;
                
                //instructions[IF].nop = TRUE;
                //instructions[ID].nop = TRUE;
                //printf("before BEQ (taken) math, imm: %d and pc: %d\n", current_int.imm,(int) mips_status->pc);
                mips_status->alu_temp = (((current_int.imm<<2) - 4) + mips_status->pc);
                printf("BEQ taken, alu temp: %d, imm: %d, imm<<2-4: %d, pc: %d\n", mips_status->alu_temp, current_int.imm, ((current_int.imm<<2) -4), mips_status->pc);
                mips_status->count_control_flow++;
            }
            else
            {
                printf("Branch BEQ is not taken\n");
                mips_status->count_control_flow++;
            }
            break;

        //JR Rs (Load the PC [program counter] with the contents of register Rs. Jump to the new
        //PC). Opcode: 010000
        case (JR):
            mips_status->jump_flag = true;
            mips_status->flushcount=2;
            //instructions[IF].nop = TRUE;
            instructions[ID].nop = TRUE;
            //printf(" Unconditional Branch is taken\n");
            mips_status->alu_temp = (rs_value +4 );
            printf("JR taken, alu temp: %d, rs: %d, rs+4: %d\n", mips_status->alu_temp, rs_value, (rs_value +4) );

            mips_status->count_control_flow++;
            break;
            
        //HALT (Stop executing the program). Opcode: 010001
        case (HALT):
            printf(" Halt case\n");
            mips_status->count_control_flow++;
            break;
            
        // Simulator shouldn't reach the default state
        default:
            //printf("Default Case reached in Execute Stage.  Something isn't working.\n");
            break;
        }
        //printf("current count: %d-------------------------------\n",mips_status->count_total);
        //printf("current pc: %d\n",mips_status->pc);
        instructions[EX] = current_int;
    }

    ex_values[1]=ex_values[0];
	// ex_values[0]->rs_value= rs_value;
    // ex_values[0]->rt_value= rt_value;
    ex_values[0].alu_temp= mips_status->alu_temp;
//    printf("nop in IF-----------------%d\n",instructions[IF].nop);
//    printf("nop in ID-----------------%d\n",instructions[ID].nop);
//    printf("nop in EX-----------------%d\n",instructions[EX].nop);
//    printf("nop in MEM-----------------%d\n",instructions[MEM].nop);
//    printf("nop in WB-----------------%d\n",instructions[WB].nop);
    return;
}
