#include "main.h"

void id_stage(inst_t instructions[], mips_status_t *mips_status, int32_t registers[], int32_t memory[])
{
    inst_t current_inst;
    current_inst = instructions[ID];

    if (current_inst.nop == true)
    {
        //do nothing
    }

    current_inst.opcode = (current_inst.binary >> 26) & 0x0000003F;

    //printf("current opcode is %x\n", current_inst.opcode);

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
        current_inst.rd_r_type = (current_inst.binary >> 11) & 0x0000001F;
        current_inst.rt_r_type = (current_inst.binary >> 16) & 0x0000001F;
        current_inst.rs_r_type = (current_inst.binary >> 21) & 0x0000001F;
        printf(" in r type, current rd %x\n", current_inst.rd_r_type);
        // printf("current rt %d\n", current_inst.rt_r_type);
        // printf("current rs %d\n", current_inst.rs_r_type);

        current_inst.valA = registers[current_inst.rs_r_type];
        current_inst.valB = registers[current_inst.rt_r_type];

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
        current_inst.imm_i_type = (current_inst.binary) & 0x00001FFF;
        current_inst.rt_i_type = (current_inst.binary >> 16) & 0x0000001F;
        current_inst.rs_i_type = (current_inst.binary >> 21) & 0x0000001F;
        // printf(" in i type, current imm %d\n", current_inst.imm_i_type);
        // printf("current rt %d\n", current_inst.rt_i_type);
        // printf("current rs %d\n", current_inst.rs_i_type);

        //check sign bit and sign extend
        signBit = (current_inst.imm_i_type >> 15) & 0x00000001;
        if (signBit == 1)
        {
            current_inst.imm_i_type = current_inst.imm_i_type | 0xFFFF0000;
            //printf("sign extended imm negative:%x\n",current_inst.imm_i_type);
        }
        else
        {
            current_inst.imm_i_type = current_inst.imm_i_type & 0x0000FFFF;
            //printf("sign extended imm positive:%x\n",current_inst.imm_i_type);
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
        //beq imm are addresses and need to be shiffted left
        if (current_inst.opcode == (BZ || BEQ || JR))
        {
            //shift left 2
            current_inst.imm_i_type = current_inst.imm_i_type << 2;

            //beq control bits, remove if not using
            // current_inst.regdst = 0;
            // current_inst.aluo1 = 0;
            // current_inst.aluo2 = 1;
            // current_inst.aluscr = 0;
            current_inst.branch = 1;
            // current_inst.memread = 0;
            // current_inst.memwrite = 0;
            // current_inst.regwrite = 0;
            // current_inst.memtoreg = 0;
        }
        break;
    }

    instructions[EX] = current_inst;
    return;
}