///////////////////////////////////////////////////////////
//
//	if.c - Instruction Fetch phase of pipeline
//
//	ECE 586 Spring 2021
//	Final Project
//
///////////////////////////////////////////////////////////

#include "main.h"

//prototypes in if.c, not accessible outside of if.c
uint32_t mux_pc(struct mips_status* status_struct, int32_t branch_signal);
void increment_pc(struct mips_status* status_struct);

//order of operations in IF: mux PC decision from previous stages, fetch value, place in register, increment pc

//fetch function: intake line from input file [PC]
    //grab pc from main.c structure - DONE
    //fetch the mem from that pc mem location - DONE
    //output hex value through to the register array, not sure which one is the IR, just using R1 for now - DONE

        //start from increment of 0 and add 1 to get to the next PC / line (since 1 line = 4 bytes) -> doing the initialization in main.c file once at the beginning before we get into the main loop


//instruction fetch function to be accessible in main.c
//pass in: filename, mainc structure with pc and pc_branch, control signal, pointer to register array
//pass out: register with needed value, R1 for now
void inst_fetch(inst_t instructions[], int32_t* registers, int32_t* memory, struct mips_status* status_struct, int32_t branch_signal, bool* hazard_flag){

    inst_t current_instruction;

    printf("pc to fetch: %d\n", status_struct->pc);

    if(*hazard_flag == true){
        printf("hazard was set => not pulling mem into IF*************\n");
    }
    else{
    current_instruction.binary = memory[status_struct->pc>>2]; //gather data from memory at chosen pc

    increment_pc(status_struct); //increment pc assuming no branch for next instruction

    instructions[IF].binary = current_instruction.binary;
    
    }
    //printf("in IF, printinf instructions[ID] %x\n", instructions[ID].binary);
    //printf("in IF, printinf instructions[IF] %x\n", instructions[IF].binary);

    return;
}

//mux function: based on control signal, choose between pc and pc_branch, output value choice; not accessible outside of if.c
//returns the PC to be used(whether this will be yes (1) or no (0) to take the branch); this needs to always happen after increment_pc from previous (so putting at beginning except for when we are reading the first line and PC == 0)
//need to be careful how we bring in control signal -> not clear in my mind yet how we will do that
//need to ensure the following: 1) no one overwrites pc after IF, 2) no one overwrites pc_branch after EX/MEM before IF and that control signal comes in for the right instruction
uint32_t mux_pc(struct mips_status* status_struct, int32_t branch_signal){

    uint32_t chosen_pc_temp;

    if(status_struct->pc == 0){ //check if this is the first line to intake, if so, then we don't need to check if we should take branch, always take line 0
        chosen_pc_temp = status_struct->pc;
        //printf("first line taken so no mux action\n");
    }
    else{
        if(branch_signal == 1){
            chosen_pc_temp = status_struct->pc_branch; //control signal of 1 indicates that we will take the branch
        }
        else if(branch_signal == 0){
            chosen_pc_temp = status_struct->pc; //control signal of 0 indicates that we will not take the branch
        }
        else{
            printf("The branch signal is an illegal value. Please debug. Default action with this bug is not to take branch.");
            chosen_pc_temp = status_struct->pc; //not taking branch
        }
    }

    status_struct->pc = chosen_pc_temp; //write the chosen value into the status struct to make sure that the right incrementation happens at the end of IF
    //printf("will return a pc choice of: %d\n", chosen_pc_temp);
    return chosen_pc_temp;
}

//increment function: take current PC and increment by 4 Bytes to point to next memory location, add 1
//to increment the pc assuming no branch will be taken; not accessible outside of if.c
void increment_pc(struct mips_status* status_struct){

    //uint32_t temp;

	status_struct->npc = status_struct->pc + 4;

//    temp = status_struct->pc;
 //   temp = temp + 1; //this is equivalent to incrementing by 4 Bytes since 1 line in the input file is equal to 4 Bytes
  //  status_struct->pc = temp;

    return;
}
