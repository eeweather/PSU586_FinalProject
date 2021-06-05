#include "main.h"

void id_stage(inst_t instructions[], mips_status_t *mips_status, int32_t registers[], int32_t memory[], int *hazard_flag, forward_stage_t *forward_stage_flag, forward_reg_t *forward_reg_flag)
{
    if ((mips_status->jump_flag == TRUE) || (mips_status->flushcount == 1))
    {
        //move down the pipeline but do nothing else
        instructions[ID] = instructions[IF];
        instructions[ID].nop = TRUE;
        mips_status->flushcount--;
    }

    else
    {
        if (*hazard_flag != true)
        {
            inst_t current_inst;
            instructions[ID] = instructions[IF];
            current_inst = instructions[ID];

            current_inst.opcode = (current_inst.binary >> 26) & 0x0000003F;

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
                current_inst.valA = registers[current_inst.rs];
                current_inst.valB = registers[current_inst.rt];
                current_inst.branch = 0;

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

                //check sign bit and sign extend
                signBit = (current_inst.imm >> 15) & 0x00000001;
                if (signBit == 1)
                {
                    current_inst.imm = current_inst.imm | 0xFFFF0000;
                }
                else
                {
                    current_inst.imm = current_inst.imm & 0x0000FFFF;
                }
                //lw control, remove if not using control bits
                if (current_inst.opcode == (LDW))
                {
                    current_inst.branch = 0;
                }
                //sw control, remove if not using control bits
                if (current_inst.opcode == STW)
                {
                    current_inst.branch = 0;
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


            // ID = i-type       
        
        //----------------------------------------------------------------------------
        // ---------------------------MEM forwarding r-type---------------------------
        //----------------------------------------------------------------------------
        if (instructions[MEM].type == 'r' && (instructions[MEM].nop == false))
        { 
        
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[MEM].rd) && (instructions[MEM].rd != 0)))
                {
                    //enable forwarding - RS in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag=1;
			mips_status->count_stall++;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;
                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");

                    }
                }
                else if(((instructions[ID].rt == instructions[MEM].rd) && (instructions[MEM].rd != 0))){
                     //enable forwarding - RT in ID stage conflicts with RD in MEM stage
                    //enable fowarding for RT from the MEM stage
                    *hazard_flag=1;
			mips_status->count_stall++;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;
                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
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
			mips_status->count_stall++;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }
                }
            }
        }    
        //----------------------------------------------------------------------------
        // ---------------------------MEM forwarding i-type---------------------------
        //----------------------------------------------------------------------------
        else if (instructions[MEM].type == 'i' && (instructions[MEM].nop == false))
        { 
        
            // ID = r-type
            if((instructions[ID].type == 'r')|| instructions[ID].opcode == BEQ)
            {
                if (((instructions[ID].rs == instructions[MEM].rt) && (instructions[MEM].rt != 0)))
                {
                    //enable forwarding - RS in ID stage conflicts with RT in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;
			mips_status->count_stall++;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB to ID rs\n");
                    }
                }
                else if (((instructions[ID].rt == instructions[MEM].rt) && (instructions[MEM].rt != 0))){
                    //enable forwarding - RT in ID stage conflicts with RT in MEM stage
                    //enable fowarding for RT from the MEM stage
                    *hazard_flag = 1;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;
			mips_status->count_stall++;

                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
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
                if((instructions[ID].rs == instructions[MEM].rt) && (instructions[MEM].rt != 0))
                {
                    //enable forwarding - RS in ID stage conflicts with RT in MEM stage
                    //enable fowarding for RS from the MEM stage
                    *hazard_flag = 1;
			mips_status->count_stall++;

                    printf("hazard_flag set---------------MEM Opcode: %d, ID Opcode: %d\n", instructions[MEM].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
			mips_status->count_stall--;
                        mips_status->count_hazards--;
                        *hazard_flag = 0;
                        *forward_stage_flag = MEM_WB;
                        *forward_reg_flag = RS;
                        printf("hazard_flag removed---------------FORWARDING from MEM_WB\n");
                    }  
                }
            }
        }                
        //----------------------------------------------------------------------------
        // ---------------------------EX forwarding r-type----------------------------
        //----------------------------------------------------------------------------
        if (instructions[EX].type == 'r' && (instructions[EX].nop == false))
        { 
        
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[EX].rd) && (instructions[EX].rd != 0)))
                {
                    // enable forwarding - RS in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
			mips_status->count_stall += 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            mips_status->count_hazards--;
				mips_status->count_stall++;
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                else if (((instructions[ID].rt == instructions[EX].rd) && (instructions[EX].rd != 0))){
                     // enable forwarding - RT in ID conflicts with RD in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
			mips_status->count_stall += 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    
                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
				mips_status->count_stall -= 2;
                            mips_status->count_hazards--;
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
			mips_status->count_stall += 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            mips_status->count_hazards--;
				mips_status->count_stall -= 2;
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                
            }
        } 
        //----------------------------------------------------------------------------
        // ---------------------------EX forwarding i-type----------------------------
        //----------------------------------------------------------------------------
        else if (instructions[EX].type == 'i' && (instructions[EX].nop == false))
        { 
            // ID = r-type
            if((instructions[ID].type == 'r') || (instructions[ID].opcode == BEQ))
            {
                if (((instructions[ID].rs == instructions[EX].rt) && (instructions[EX].rt != 0)))
                {
                    // enable forwarding - RS in ID conflicts with RT in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
			mips_status->count_stall += 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            mips_status->count_hazards--;
				mips_status->count_stall -= 2;
                            *hazard_flag = 0;
                            *forward_stage_flag = EX_MEM;
                            *forward_reg_flag = RS;
                            printf("hazard_flag removed---------------FORWARDING from EX_MEM\n");
                        }
                    }
                }
                else if(((instructions[ID].rt == instructions[EX].rt) && (instructions[EX].rt != 0))){
                     // enable forwarding - RT in ID conflicts with RT in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
			mips_status->count_stall += 2;

                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            mips_status->count_hazards--;
				mips_status->count_stall -= 2;
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
                    // enable forwarding - RS in ID conflicts with RT in EX stage
                    // If not a LDW command then enable forwarding for RS
                    *hazard_flag = 2;
			mips_status->count_stall += 2;
                    printf("hazard_flag set---------------EX Opcode: %d, ID Opcode: %d\n", instructions[EX].opcode, instructions[ID].opcode);
                    mips_status->count_hazards++;

                    if (mips_status->mode == FORWARDING)
                    {
                        if (instructions[EX].opcode == LDW)
                        { //if it is a LDW command, then wait a cycle
                            *hazard_flag = 1;
				mips_status->count_stall--;
                            mips_status->lwd_stall_flag = true;
                        }
                        else
                        {
                            mips_status->count_hazards--;
				mips_status->count_stall -= 2;
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
