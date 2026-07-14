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


void read_image_file (FILE* file);
int read_image(const char* image_path);

uint16_t mem_read (uint16_t address);
void mem_write (uint16_t address, uint16_t val);

void to_big_endian (uint16_t x);


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

  int running = 1;

  while (running)
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
		    trap_in();
		    break;

		  case TRAP_PUTSP:
		    trap_putsp();
		    break;

		  case TRAP_HALT:
		    trap_halt();
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

void read_image_file (FILE* file)
{
  
  uint16_t* origin;
  fread (origin, sizeof(origin), 1, file);
  to_big_endian (*origin);

  uint16_t mem_read = MAX_MEMORY - origin;
  uint16_t* p = memory + origin;
  fread (p, sizeof(p), mem_read, file); 

  while (mem_read-- > 0)
  {
    to_big_endian (*p);
    p++;
  }
}

int read_image(const char* image_path)
{
    FILE* file = fopen(image_path, "rb");
    if (!file) { return 0; };
    read_image_file(file);
    fclose(file);
    return 1;
}

void to_big_endian (uint16_t x)
{
  return (x << 8) | (x >> 8);
}

void mem_write (uint16_t address, uint16_t val)
{
  memory[address] = val;
}

uint16_t mem_read (uint16_t address)
{
  if (address == KBSR)
  {
    if (check_key())
    {
      memory[KBSR] = (1 << 15);
      memory[KBDR] = getchar();
    }
    else
    {
      memory[KBSR] = 0;
    }
  }
  return memory[address];}

