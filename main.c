#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>


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


int main (int agrc, char* argv[])
{
  if (agrc < 2)
  {
    printf ("Usage: vm [image-file] ... \n");
    exit (2);
  }
  else 
  {
    for (int j = 1; j < argc, j++)
    {
      if (!read_image(agrv[j]))
      {
        printf ("Failed to load image %s\n", argv[j]);
        exit(1);
      }
    }
  }

  // load the instruction in the register
  
  // Set the Zero conidition flag 
  *COND = ZRO_F;

  // Set the PC to 0x3000, it will be its starting position
  *PC = 0x3000;

  whlie (1)
  {
    unint16_t instruction = mem_read ((*PC) + 1); // read the next instruction in the program counter
    
    switch (instruction)
    {
      case OP_BR:
        // OP_BR
        break;
      
      case OP_ADD:
        // OP_ADD 
        break;
      
      case OP_LD:
        // OP_LD 
        break;
      
      case OP_ST:
        // OP_ST 
        break;
      
      case OP_JSR:
        // OP_JSR 
        break;
      
      case OP_AND:
        // OP_AND
        break;
      
      case OP_LDR:
        // OP_LDR
        break;
      
      case OP_STR:
        // OP_STR
        break;
      
      case OP_NOT:
        // OP_NOT
        break;
      
      case OP_LDI:
        // OP_LDI
        break;
      
      case OP_STI:
        // OP_STI
      
      case OP_JMP:
        // OP_JMP
        break;
      
      case OP_LEA:
        // OP_LEA
        break;
      
      case OP_TRAP:
        // OP_LEA
        break;
      
      case OP_RES: /* The reserved OPCODES */
      case OP_RTI: 
      default:
        
        break;
      
    }
  }

}
