/*

Hazards RAW

1. no forwarding
    pc takes 5 cycles for 1 inst, 1 per each

                1 2 3 4 5 6 7 8 9 10 11 12 13
add r1 r2 r3    f d e m w
dep on r1         f s s d e m w
not dep                 f d e m w

add r1 r2 r3    f d e m w
not dep           f d e m w
dep on r1           f s d e m w


dep on r1

*/

#include <stdbool.h>

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

//all stages start locked except IF
bool iflock = false;
bool idlock = true;
bool exlock = true;
bool memlock = true;
bool wblock = true;

//make sure all nops =1, except if

int main()
{

    while (1)
    {
        if (!wblock)
        {
            run WB
        }
        if (!memlock)
        {
            run MEM
            wblock = false;
        }

        if (!exlock)
        {
            run EX
            if rd is rs for IF 
                stall1 = true 
            memlock = false;
        }

        if (!idlock)
        {
            if(stall1)
                NOP
            else
                if rd is rs for IF 
                stall2 = true
                run ID
            
            exlock = false;
        }

        if (!iflock)
        {
             if(stall1)
                NOP
            else if (stall2)
                stall1 = true 
            else
                run IF
            idlock = false;
        }
    }
}