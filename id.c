#include "main.h"

void id_stage(inst_t instructions[], mips_status_t *mips_status, int32_t registers[], int32_t memory[], int *hazard_flag, forward_stage_t *forward_stage_flag, forward_reg_t *forward_reg_flag)
{
    if ((mips_status->jump_flag == TRUE) || (mips_status->flushcount == 1))
    {
        //move down the pipeline but do nothing else
        //printf("flush count start nop set: %d\n", mips_status->flushcount);
        //printf("setting nop in id\n");
        instructions[ID] = instructions[IF];
        instructions[ID].nop = TRUE;
        mips_status->flushcount--;
        //printf("flush count after nop set: %d\n", mips_status->flushcount);
        // if(mips_status->flushcount==0){
        //     mips_status->flushcount=2;
        // }
    }

    else
    {
        //printf("hazard flag value at the beginning of ID: ");
        //printf(*hazard_flag ? "hazard true\n" : "no hazard\n");

        if (*hazard_flag != true)
        {
            inst_t current_inst;
            instructions[ID] = instructions[IF];
            current_inst = instructions[ID];

            current_inst.opcode = (current_inst.binary >> 26) & 0x0000003F;

            //printf("current opcode in ID is %x\n", current_inst.opcode);

            int signBit;

            switch (current_inst.opcode)
            {
            case ADD:
            case SUB:
            case MUL:
            case OR:
            case AND:
            case XOR:
                current_inst.type = 'r';
                current_inst.rd = (current_inst.binary >> 11) & 0x0000001F;
                current_inst.rt = (current_inst.binary >> 16) & 0x0000001F;
                current_inst.rs = (current_inst.binary >> 21) & 0x0000001F;
                //printf(" in r type, current rd %x\n", current_inst.rd);
                // printf("current rt %d\n", current_inst.rt);
                // printf("current rs %d\n", current_inst.rs);

                current_inst.valA = registers[current_inst.rs];
                current_inst.valB = registers[current_inst.rt];

                //r command control bits, remove if not using
                // current_inst.regdst = 1;
                // current_inst.aluo1 = 1;
                // current_inst.aluo2 = 0;
                // current_inst.aluscr = 0;
                current_inst.branch = 0;
                // current_inst.memread = 0;
                // current_inst.memwrite = 0;
                // current_inst.regwrite = 1;
                // current_inst.memtoreg = 0;

                break;
            case ADDI:
            case SUBI:
            case MULI:
            case ORI:
            case ANDI:
            case XORI:
            case LDW:
            case STW:
            case BZ:
            case BEQ:
            case JR:
            case HALT:
                current_inst.type = 'i';
                current_inst.imm = (current_inst.binary) & 0x0000FFFF;
                current_inst.rt = (current_inst.binary >> 16) & 0x0000001F;
                current_inst.rs = (current_inst.binary >> 21) & 0x0000001F;
                // printf(" in i type, current imm %d\n", current_inst.imm);
                // printf("current rt %d\n", current_inst.rt);
                // printf("current rs %d\n", current_inst.rs);

                //check sign bit and sign extend
                signBit = (current_inst.imm >> 15) & 0x00000001;
                if (signBit == 1)
                {
                    current_inst.imm = current_inst.imm | 0xFFFF0000;
                    //printf("sign extended imm negative:%x\n",current_inst.imm);
                }
                else
                {
                    current_inst.imm = current_inst.imm & 0x0000FFFF;
                    //printf("sign extended imm positive:%x\n",current_inst.imm);
                }
                //lw control, remove if not using control bits
                if (current_inst.opcode == (LDW))
                {
                    // current_inst.regdst = 0;
                    // current_inst.aluo1 = 0;
                    // current_inst.aluo2 = 0;
                    // current_inst.aluscr = 1;
                    current_inst.branch = 0;
                    // current_inst.memread = 1;
                    // current_inst.memwrite = 0;
                    // current_inst.regwrite = 1;
                    // current_inst.memtoreg = 1;
                }
                //sw control, remove if not using control bits
                if (current_inst.opcode == STW)
                {
                    // current_inst.regdst = 0;
                    // current_inst.aluo1 = 0;
                    // current_inst.aluo2 = 0;
                    // current_inst.aluscr = 1;
                    current_inst.branch = 0;
                    // current_inst.memread = 0;
                    // current_inst.memwrite = 1;
                    // current_inst.regwrite = 0;
                    // current_inst.memtoreg = 0;
                }
                //beq imm are addresses and need to be shifted left
                if (current_inst.opcode == BZ || current_inst.opcode == BEQ || current_inst.opcode == JR)
                {
                    current_inst.branch = 1;
                }
                break;
            }
            //printf("ID opcode, %d.\n",current_inst.opcode);
            instructions[ID] = current_inst;
        }

        //RAW HAZARD Detection //
        // Check to see if the either source register in ID matches the destionation registers in EX or MEM
        // Also check to make sure the destionation registers aren't just R0
        // If true,  trigger the raw flag indicating the hazard
        *hazard_flag = false;
        //printf("Instruction type: %d\n",instructions[ID].type);
        //printf("source: %d & previous EX destination: %d & previous MEM destination: %d\n", instructions[ID].rs, instructions[EX].rd, instructions[MEM].rd);
        
        
            // ID = i-type       
        
        
        // MEM forwarding r-type
        
        if (instructions[MEM].type == 'r' && (instructions[MEM].nop == false))
        { //r-type is 114
        
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[MEM].rd) && (instructions[MEM].rd != 0)))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag=1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }
                }
                else if(((instructions[ID].rt == instructions[MEM].rd) && (instructions[MEM].rd != 0))){
                     //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag=1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RT;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }
                }
            }
            // ID = i-type
            else if((instructions[ID].type == 'i'))
            {
                if ((instructions[ID].rs == instructions[MEM].rd) && (instructions[MEM].rd != 0))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }
                }
            }
        }    
        // MEM forwarding i-type
        else if (instructions[MEM].type == 'i' && (instructions[MEM].nop == false))
        { //r-type is 114
        
            // ID = r-type
            if((instructions[ID].type == 'r'))
            {
                if (((instructions[ID].rs == instructions[MEM].rt) && (instructions[MEM].rt != 0)))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB to ID rs\n");
                    }
                }
                else if (((instructions[ID].rt == instructions[MEM].rt) && (instructions[MEM].rt != 0))){
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RT;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB to ID rt\n");
                    }
                }
            }
            // ID = i-type
            else if((instructions[ID].type == 'i'))
            {
                if ((instructions[ID].rs == instructions[MEM].rs) && (instructions[MEM].rs != 0))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }
                }
                else if((instructions[ID].rs == instructions[MEM].rt) && (instructions[MEM].rt != 0))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }  
                }
            }
        }                
  
        // EX forwarding r-type
        if (instructions[EX].type == 'r' && (instructions[EX].nop == false))
        { //r-type is 114
        
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[EX].rd) && (instructions[EX].rd != 0)))
                {
                    // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                else if (((instructions[ID].rt == instructions[EX].rd) && (instructions[EX].rd != 0))){
                     // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RT;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
            }    
            // ID = i-type
            else if((instructions[ID].type == 'i'))
            {
                if ((instructions[ID].rs == instructions[EX].rd) && (instructions[EX].rd != 0))
                {
                    // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                
            }
        } 
        // EX forwarding i-type
        else if (instructions[EX].type == 'i' && (instructions[EX].nop == false))
        { //r-type is 114
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[EX].rt) && (instructions[EX].rt != 0)))
                {
                    // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                else if(((instructions[ID].rt == instructions[EX].rt) && (instructions[EX].rt != 0))){
                     // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RT;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
            } 
            else if((instructions[ID].type == 'i'))
            {
                if ((instructions[ID].rs == instructions[EX].rt) && (instructions[EX].rt != 0))
                {
                    // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                
            }
        }    
        //printf(*hazard_flag ? "hazard true\n" : "no hazard\n");
    }
    if (*hazard_flag > 0)
    {
        //do nothing
        instructions[ID].nop = true;
        //printf("nop in ID---------------\n");
    }
    else
    {
        instructions[ID].nop = false;
        *hazard_flag = 0;
    }

    printf("Hazard flags in play: %d\n",*hazard_flag);

    return;
}
