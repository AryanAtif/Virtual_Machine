#include <stdio.h>
#include <stdlib.h>


// We shall keep the max memory to be 16-bit
#define MEMORY_CAPACITY (1 << 16)

uint16_t* memory = malloc (MEMORY_CAPACITY * sizeof (uint16_t));

/*
 * Registers:
 *    - There are 8 GPRs, 1 PC register, and 1 Conditional flag register
 *    - Each register is 16-bit long
 */
#define NUM_GPR 8 
unint16_t* GPR = malloc ( NUM_GPR * sizeof (unint16_t));

unint16_t* PC = malloc (sizeof (unint16_t));
unint16_t* COND = malloc (sizeof (unint16_t));

/*
 * OPCODES:
 *    - There are 16 OPCODES
 *    - Each of which is of 4-bits
 */

enum 
{
  OP_BR = 0,  // Branch
  OP_ADD,     // Add
  OP_LD,      // Load
  OP_ST,      // Store
  OP_JSR,     // Jump Register 
  OP_AND,     // Bitwise And 
  OP_LDR,     // Load Register 
  OP_STR,     // Store Register 
  OP_RTI,     // unused
  OP_NOT,     // Bitwise NOT
  OP_LDI,     // Load indirect 
  OP_STI,     // Store indirect
  OP_JMP,     // Jump
  OP_RES,     // Reserved
  OP_LEA,     // Load effective address
  OP_TRAP,    // Execute Trap
};

/*
 * Condition Flags
 *    - There are 3 condition flags
 */

enum 
{
  POS_F = 1 << 0;  // x x 1
  ZRO_F = 1 << 1;  // x 1 x
  NEG_F = 1 << 2;  // 1 x x
};














