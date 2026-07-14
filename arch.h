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
 * Memory Mapped Registers
 *    - Keyboard Status Register 
 *    - Keyboard Data Register
 */

enum 
{
  KBSR = 0xFE00, // Keyboard status register 
  KBDR = 0xFE02 // Keyboard data register 
}



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
 * Trap Routines
 */

enum 
{
  TRAP_GETC = 0x20,    // Get a character
  TRAP_OUT = 0x21,     // Output a character 
  TRAP_PUTS = 0x22,    // Print a word string 
  TRAP_IN = 0x23,      // Input a character, echo it to the terminal 
  TRAP_PUTSP = 0x24,   // Print a byte string
  TRAP_HALT = 0x25,    // Halt the program
}

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
