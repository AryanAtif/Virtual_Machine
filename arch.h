#include <stdint.h>

// We shall keep the max memory to be 16-bit
#define MAX_MEMORY (1 << 16)

uint16_t memory [MAX_MEMORY];

/*
 * Registers:
 *    - There are 8 GPRs, 1 PC register, and 1 Conditional flag register
 *    - Each register is 16-bit long
 */
#define NUM_GPR 8 
uint16_t GPR [NUM_GPR];
uint16_t PC;
uint16_t COND;

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
 *    - Any flag set, will be stored in COND
 */

enum 
{
  POS_F = 1 << 0,  // x x 1
  ZRO_F = 1 << 1,  // x 1 x
  NEG_F = 1 << 2,  // 1 x x
};
