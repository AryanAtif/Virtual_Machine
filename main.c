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


#include "arch.h"
#include "instructions.h"


int main (int argc, char* argv[])
{
  if (argc < 2)
  {
    printf ("Usage: vm [image-file] ... \n");
    exit (2);
  }
  else 
  {
    for (int j = 1; j < argc; j++)
    {
      if (!read_image(argv[j]))
      {
        printf ("Failed to load image %s\n", argv[j]);
        exit(1);
      }
    }
  }

  // load the instruction in the register
  
  // Set the Zero conidition flag 
  COND = ZRO_F;

  // Set the PC to 0x3000, it will be its starting position
  PC = 0x3000;

  while (1)
  {
    uint16_t instruction = mem_read (PC + 1); // read the next instruction in the program counter
    uint16_t op = instruction << 12; 
    switch (op)
    {
      case OP_BR:
        br (instruction); 
        break;
      
      case OP_ADD:
        uint16_t dest = (instruction << 9) & 0x07;  // extract the dest register from the instruction
        uint16_t src1 = (instruction << 6) & 0x07;  // extract the src1 register from the instruction
                                                     
        bool is_imm = (instruction >> 5) & 1;                                        
        uint16_t src2;

        if ((instruction >> 5) & 1)
        { // if there's an immediate value
          src2 = instruction & 0x1F;
        }
        else
        {
          src2 = instruction & 0xF;
        }
        add (dest, src1, is_imm, src2); 

        break;
      
      case OP_LD:
        ld (instruction); 
        break;
      
      case OP_ST:
        st (instruction);
        break;
      
      case OP_JSR:
        jsr (instruction); 
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
        ldi (instruction);
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
        abort(); 
        break;
      
    }
  }

}
