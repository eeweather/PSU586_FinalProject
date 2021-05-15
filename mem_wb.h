#ifndef _MEM_WB_H
#define _MEM_WB_H

#include "main.h"

// function prototypes
void memory_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[]);
void writeback_stage(inst_t instructions[], mips_status_t* mips_status, int32_t registers[], int32_t memory[]);

#endif
