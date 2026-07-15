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


/*Fixes for Linux, no need to peak here, go to the main function to see how the program works*/

struct termios original_tio;

void disable_input_buffering()
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

uint16_t check_key()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}


void handle_interrupt(int signal);

/************************************************************************************************/

// Global Variables

int running = 1;
uint16_t GPR [NUM_GPR];
uint16_t PC;
uint16_t COND;
uint16_t memory [MAX_MEMORY];




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
  
  signal(SIGINT, handle_interrupt);
  disable_input_buffering();


  // load the instruction in the register
  
  // Set the Zero conidition flag 
  COND = ZRO_F;

  // Set the PC to 0x3000, it will be its starting position
  PC = 0x3000;

  int i = 1;
  while (running)
  {
    printf ("======================\n Status after %d iteration:\n", i); 
    printf ("R0 = %d or %c\n", GPR[0], GPR[0]); 
    i++;
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
      
      case OP_RES: /* The reserved OPCODES */
      case OP_RTI: 
      default:
        abort(); 
        break;
    
    printf ("======================\n Status after %d iteration:\n", i); 
    printf ("R0 = %d or %c\n", GPR[0], GPR[0]); 


    } 
  }
  restore_input_buffering(); 
}

void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

