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
        and (instruction);
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
        and (instruction);
        break;
      
      case OP_LDR:
        ldr (instruction); 
        break;
      
      case OP_STR:
        str (instruction); 
        break;
      
      case OP_NOT:
		not (instruction); 
        break;
      
      case OP_LDI:
        ldi (instruction);
        break;
      
      case OP_STI:
        sti (instruction);
        break;

      case OP_JMP:
        jmp (instruction);
        break;
      
      case OP_LEA:
        lea (instruction);
        break;
      
      case OP_TRAP:
		switch (instruction & 0xff)
		{
		  case TRAP_GETC:
		    trap_getc();	
		    break;

		  case TRAP_OUT:
		    trap_out();
		    break;

		  case TRAP_PUTS:
		    trap_puts();
		    break;

		  case TRAP_IN:

		    break;

		  case TRAP_PUTSP:

		    break;

		  case TRAP_HALT:

		    break;
		}
        break;
      
      case OP_RES: /* The reserved OPCODES */
      case OP_RTI: 
      default:
        abort(); 
        break;
      
    }
  }

}
