///////////////////////////////////////////////////////////
//
//	ie.c - Instruction Execute phase of pipeline
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

//instruction structs
typedef struct inst
{
    int binary; //maybe dont need
    char type;
    int opcode;
    int rs_r_type;  //created separate rs and rt for instruction types
    int rt_r_type;
    int rd_r_type;
    int rs_i_type;
    int rt_i_type;
    int imm_i_type;
    int valA;
    int valB;
    bool regdst; //don't know if anyone needs the control bits but I'm adding them
    bool aluo1;  //can take them out if no one needs them
    bool aluo2;
    bool aluscr;
    bool branch;
    bool memread;
    bool memwrite;
    bool regwrite;
    bool memtoreg;
} inst_t;

// MIPS architecture struct
typedef struct mips_status
{
    bool debug; // whether program is in debug mode
    int mode;   // which simulation mode the program is in
    uint32_t pc;        
    uint32_t npc;
    uint32_t pc_branch; 
    uint32_t alu_temp;  
    uint32_t mem_reg;   
    uint32_t temp_pc;   
    uint16_t count_total;
    uint16_t count_arith;
    uint16_t count_logic;
    uint16_t count_memory_access;
    uint16_t count_control_flow;
    bool zero_flag;         
    bool jump_flag;         
    bool halt;
} mips_status_t;  

int32_t registers[32];


void execution_stage (struct mips_core *mips_status_t)
{
    
    switch(inst_t->opcode)
    {
        //ADD Rd Rs Rt (Add the contents of registers Rs and Rt, transfer the result 
        //to register Rd). Opcode: 000000
        case(ADD):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] + registers[inst_t->rt_r_type];
        printf("Value of Temp register in ADD: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;
        
        //ADDI Rt Rs Imm (Add the contents of register Rs to the immediate value “Imm”,
        //transfer the result to register Rt). Opcode: 000001
        case(ADDI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] + inst_t->imm_i_type;
        printf("Value of Temp register in ADDI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;

        //SUB Rd Rs Rt (Subtract the contents of register Rt from Rs, transfer the result to
        //register Rd). Opcode: 000010
        case(SUB):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] - registers[inst_t->rt_r_type];
        printf("Value of Temp register in SUB: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;

        //SUBI Rt Rs Imm (Subtract the immediate value “Imm” from the contents of register Rs,
        //transfer the result to register Rt). Opcode: 000011
        case(SUBI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] - inst_t->imm_i_type;
        printf("Value of Temp register in SUBI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;
        
        //MUL Rd Rs Rt (Multiply the contents of registers Rs and Rt, transfer the result to
        //register Rd). Opcode: 000100
        case(MUL):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] * registers[inst_t->rt_r_type];
        printf("Value of Temp register in MUL: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;

        //MULI Rt Rs Imm (Multiply the contents of registers Rs with the immediate value
        //“Imm”, transfer the result to register Rt). Opcode: 000101
        case(MULI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] * inst_t->imm_i_type;
        printf("Value of Temp register in MULI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_arith++;
        mips_status_t->count_total++;
        break;

        //OR Rd Rs Rt (Take a bitwise OR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 000110
        case(OR):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] | registers[inst_t->rt_r_type];
        printf("Value of Temp register in logical OR: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;

        //ORI Rt Rs Imm (Take a bitwise OR of the contents of registers Rs and the immediate
        //value “Imm”, transfer the result to register Rt). Opcode: 000111
        case(ORI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] | inst_t->imm_i_type;
        printf("Value of Temp register in logical ORI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;
        
        //AND Rd Rs Rt (Take a bitwise AND of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001000
        case(AND):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] & registers[inst_t->rt_r_type];
        printf("Value of Temp register in logical AND: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;

        //ANDI Rt Rs Imm (Take a bitwise AND of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001001
        case(ANDI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] & inst_t->imm_i_type;
        printf("Value of Temp register in logical ANDI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;

        //XOR Rd Rs Rt (Take a bitwise XOR of the contents of registers Rs and Rt, transfer the
        //result to register Rd). Opcode: 001010
        case(XOR):
        mips_status_t->alu_temp =  registers[inst_t->rs_r_type] ^ registers[inst_t->rt_r_type];
        printf("Value of Temp register in logical XOR: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;

        //XORI Rt Rs Imm (Take a bitwise XOR of the contents of registers Rs and the
        //immediate value “Imm”, transfer the result to register Rt). Opcode: 001011
        case(XORI):
        mips_status_t->alu_temp =  registers[inst_t->rs_i_type] ^ inst_t->imm_i_type;
        printf("Value of Temp register in logical XORI: %d\n",mips_status_t->alu_temp);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_logic++;
        mips_status_t->count_total++;
        break;

        //LDW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, load the contents (32-bits) of the memory location at address
        //“A” into register Rt). Opcode: 001100
        case(LDW):
        mips_status_t->mem_reg = registers[inst_t->rs_i_type] + inst_t->imm_i_type;
        printf("Value of the register rs in r type:%d\n",registers[inst_t->rs_r_type]);
        printf("Value of the register rs in i type: %d\n",inst_t->rs_i_type);
        printf("Value of register type: %d\n",inst_t->rs_r_type);
        printf("Value of the register r1 in array:%d\n",registers[1]);
        printf("Final value of the mem_reg variable: %d\n",mips_status_t->mem_reg);
        mips_status_t->mem_reg = (mips_status_t->mem_reg / 4);
        printf("Final value of the mem_reg variable after dividing it by 4: %d\n",mips_status_t->mem_reg);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_total++;
        mips_status_t->count_memory_access++;
        break;

        //STW Rt Rs Imm (Add the contents of Rs and the immediate value “Imm” to generate
        //the effective address “A”, store the contents of register Rt (32-bits) at the memory
        //address “A”). Opcode: 001101
        case(STW):
        mips_status_t->mem_reg = registers[inst_t->rs_i_type] + inst_t->imm_i_type;
        mips_status_t->mem_reg = (mips_status_t->mem_reg / 4);
        mips_status_t->pc = mips_status_t->pc + 1;
        mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
        mips_status_t->count_total++;
        mips_status_t->count_memory_access++;
        break;

        //BZ Rs x (If the contents of register Rs are zero, then branch to the “x”th instruction from
        //the current instruction). Opcode: 001110
        case(BZ):
        if((registers[inst_t->rs_i_type]) == 0)
        {
            mips_status_t->zero_flag = true;
            printf("Conditional BZ  Branch is taken\n");
            mips_status_t->pc = (inst_t->imm_i_type + mips_status_t->pc);
            mips_status_t->temp_pc = (mips_status_t->pc * 4);
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
        }
        else
        {
            printf("Branch BZ is not taken\n");
            mips_status_t->pc = mips_status_t->pc + 1;
            mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
        }
        break;
        
        //BEQ Rs Rt x (Compare the contents of registers Rs and Rt. If they are equal, then
        //branch to the “x”th instruction from the current instruction). Opcode: 001111
        case(BEQ):
        if( (registers[inst_t->rt_i_type]) == (registers[inst_t->rs_i_type]) )  
        {
            mips_status_t->zero_flag = true;
            printf("Conditional BEQ Branch is taken\n");
            mips_status_t->pc = (inst_t->imm_i_type + mips_status_t->pc);
            mips_status_t->temp_pc = (mips_status_t->pc * 4);
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
        }
        else
        {
            printf("Branch BEQ is not taken\n");
            mips_status_t->pc = mips_status_t->pc + 1;
            mips_status_t->temp_pc = mips_status_t->temp_pc + 4;
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
        }
        break;
        
        //JR Rs (Load the PC [program counter] with the contents of register Rs. Jump to the new
        //PC). Opcode: 010000
        case(JR):
            mips_status_t->jump_flag = true;
            printf(" Unconditional Branch is taken\n");
            mips_status_t->temp_pc  = registers[inst_t->rs_i_type];
            mips_status_t->pc = (mips_status_t->temp_pc / 4);
            mips_status_t->count_control_flow++;
            mips_status_t->count_total++;
        break;
        
        // Simulator shouldn't reach the default state
        default:
        printf("Default Case reached in Execute Stage.  Something isn't working.\n");
        break;

    }
}
