//Id stsage of pipeline

// libraries and includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

//instruction defines
#define IF 0
#define ID 1
#define EX 2
#define MEM 3
#define WB 4

//instruction structs
typedef struct inst
{
    int binary; //maybe dont need
    char type;
    int opcode;
    int rs;
    int rt;
    int rd;
    int imm;
    int valA;
    int valB;
    bool nop;
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

int32_t memory[1024];
inst_t instructions[5];
int32_t registers[32];

void id_stage(inst_t instructions[], mips_status_t *mips_status, int32_t registers[], int32_t memory[])
{
    if nop == true {
        
    }
    inst_t current_inst = instructions[IF];
    int signBit;

    switch (current_inst.type)
    {
    case 'r':
        //if registers values not stored yet store register via:
        /*
            current_inst.rd = (current_inst.binary >> 11) & 0x0000001F;
            current_inst.rt = (current_inst.binary >> 16) & 0x0000001F;
            current_inst.rs = (current_inst.binary >> 21) & 0x0000001F;
            current_inst.opcode = (current_inst.binary >> 26) & 0x0000003F;
            */

        current_inst.valA = registers[current_inst.rs];
        current_inst.valB = registers[current_inst.rt];

        //r command control bits, remove if not using
        current_inst.regdst = 1;
        current_inst.aluo1 = 1;
        current_inst.aluo2 = 0;
        current_inst.aluscr = 0;
        current_inst.branch = 0;
        current_inst.memread = 0;
        current_inst.memwrite = 0;
        current_inst.regwrite = 1;
        current_inst.memtoreg = 0;

        break;
    case 'i':
        //if registers values not stored yet store register via:
        /*
            current_inst.imm = (current_inst.binary) & 0x00001FFF;
            current_inst.rt = (current_inst.binary >> 16) & 0x0000001F;
            current_inst.rs = (current_inst.binary >> 21) & 0x0000001F;
            current_inst.opcode = (current_inst.binary >> 26) & 0x0000003F;
            */

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
        if (current_inst.opcode == (0b001100))
        {
            current_inst.regdst = 0;
            current_inst.aluo1 = 0;
            current_inst.aluo2 = 0;
            current_inst.aluscr = 1;
            current_inst.branch = 0;
            current_inst.memread = 1;
            current_inst.memwrite = 0;
            current_inst.regwrite = 1;
            current_inst.memtoreg = 1;
        }
        //sw control, remove if not using control bits
        if (current_inst.opcode == 0x001101)
        {
            current_inst.regdst = 0;
            current_inst.aluo1 = 0;
            current_inst.aluo2 = 0;
            current_inst.aluscr = 1;
            current_inst.branch = 0;
            current_inst.memread = 0;
            current_inst.memwrite = 1;
            current_inst.regwrite = 0;
            current_inst.memtoreg = 0;
        }
        //beq imm are addresses and need to be shiffted left
        if (current_inst.opcode == (0x001110 || 0x010000))
        {
            //shift left 2
            current_inst.imm = current_inst.imm << 2;

            //beq control bits, remove if not using
            current_inst.regdst = 0;
            current_inst.aluo1 = 0;
            current_inst.aluo2 = 1;
            current_inst.aluscr = 0;
            current_inst.branch = 1;
            current_inst.memread = 0;
            current_inst.memwrite = 0;
            current_inst.regwrite = 0;
            current_inst.memtoreg = 0;
        }
        break;
    case 'j':
        //I think we have no j instructions in mips light
        break;
    }

    instructions[ID]=current_inst;
}