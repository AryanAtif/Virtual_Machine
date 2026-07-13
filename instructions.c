#include <stdint.h>

#include "arch.h"
#include "instructions.h"

// See instructions.h for the description of each of the functions defined in this file

/* =================
 * Helper Functions
 * ================= 
 */

uint16_t sign_extend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) 
    {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

void set_flag (uint16_t reg)
{
  if (GPR[reg] == 0) // if the register contains nothing
  {
    COND = ZRO_F; 
  }
  else if (GPR[reg] & (1 << 16)) // if the MSB of the register (the sign bit) == 1
  {
    COND = NEG_F; 
  }
  else 
  {
    COND = POS_F;
  }
}

/*
 * =============
 * Instructions
 * =============
 */

void add (uint16_t instruction)
{
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

  GPR [src1] = sign_extend (GPR[src1], 5);

  if (is_imm) // if src2 is an immediate value
  {
    GPR[dest] = GPR[src1] + src2; 
  }
  else 
  {
    GPR [src2] = sign_extend (GPR[src2], 5);
    GPR[dest] = GPR [src1] + GPR[src2];
  }

  set_flag(dest);
}

void ldi (uint16_t instruction) 
{
  // Get the destination register
  uint16_t dest = (instruction >> 9) & 0x7;
  // Get PCOFFSET9
  uint16_t pc_offset = instruction & 0x1FF; 

  // we need to sign extend pcoffset (9-bit) to 16-bits
  pc_offset = sign_extend(pc_offset, 16);

  int mem_to_read = mem_read(PC + pc_offset);

  GPR [dest] = mem_read (mem_to_read); // mem_to_read is itself an address to a value

  set_flag(dest);
}

void ld (uint16_t instruction)
{
  uint16_t dest = (instruction >> 9) & 0x7;
  // Get PCOFFSET9
  uint16_t pc_offset = instruction & 0x1FF; 

  // we need to sign extend pcoffset (9-bit) to 16-bits
  pc_offset = sign_extend(pc_offset, 16);

  int to_read = mem_read(PC + pc_offset);

  GPR[dest] = to_read; 

  set_flag(dest);

}

void ldr (uint16_t instruction)
{
  uint16_t dest = (instruction >> 9) & 0x7;
  uint16_t base_r = (instruction >> 6) & 0x7;
  uint16_t offset = instruction & 0x3F; 

  offset = sign_extend(offset, 16);

  GPR[dest] = mem_read (GPR[base_r] + offset);

  set_flag(dest);
}

void lea (uint16_t instruction)
{
  uint16_t dest = (instruction >> 9) & 0x7;
  uint16_t pc_offset = instruction & 0x7F; 

  offset = sign_extend(offset, 16);

  GPR[dest] = PC + pc_offset;
}

void br (uint16_t instruction)
{
  uint16_t flag_given = (instruction >> 9) & 0x7;
  // 100 = n
  // 010 = p
  // 001 = z
  // 000 = branch out unconditionaly  

  bool is_flag_set = ((flag_given == 0b100) & NEG_F) || ((flag_given == 0b010) & POS_F) || ((flag_given == 0b001) & ZRO_F);

  if (is_flag_set)
  {
    uint16_t pc_offset = instruction & 0x1FF;
    pc_offset = sign_extend(pc_offset, 16);

    PC = pc_offset;
  }
  else if (flag_given == 0b000)
  {
    PC++;
  }

}

void st (uint16_t instruction)
{
  
  uint16_t src1 = (instruction >> 9) & 0x7;
  uint16_t pc_offset = instruction & 0x1FF;

  pc_offset = sign_extend(pc_offset, 16);

  mem_read (PC + pc_offset) = GPR[src1];

  set_flag(src);
}

void str (uint16_t instruction)
{
  uint16_t src1 = (instruction >> 9) & 0x7;
  uint16_t base_r = (instruction >> 6) & 0x7;
  uint16_t offset = instruction & 0x3F;

  offset = sign_extend(offset, 16);

  mem_read (GPR[base_register] + offset) = GPR[src1];
  set_flag(src);
}

void sti (uint16_t instructions)
{
  uint16_t src = (instruction >> 9) & 0x7;
  uint16_t pc_offset = instruction & 0x1FF; 

  pc_offset = sign_extend(pc_offset, 16);

  int mem_to_read = mem_read(PC + pc_offset);

  mem_read (mem_to_read) = GPR[src]; 

  set_flag(src);
}

void jsr (uint16_t instruction)
{
 bool is_label = (instruction >> 11) & 1;

 if (is_label)
 {
    uint16_t pc_offset = instruction & 0x7FF;
    pc_offset = sign_extend(pc_offset, 16);
    PC = PC + pc_offset;
 }
 else 
 {
   uint16_t base_r = (instruction >> 6) & 0x7;
   PC = GPR[base_r];
 }
}

void jmp (uint16_t instruction)
{
  uint16_t base_r =  (instruction >> 6) & 0x7;
  PC = GPR[base_r];
}

void trap (uint16_t instruction)
{
  trap_vect = instruction & 0xFF;

  trap_vect = sign_extend (trap_vect, 16);

  GPR[7] = PC;  
  PC = mem_read(trap_vect);
}

void and (uint16_t instruction)
{
 uint16_t dest = (instruction >> 9) & 0x7;
 uint16_t src1 = (instruction >> 6) & 0x7;

 if ((instruction >> 5) & 1) // if there's an immediate value in the instruction
 {
   uint16_t imm = instruction & 0x1F;
   GPR[dest] = GPR[src1] & imm; 
 }
 else
 {
   uint16_t src2 = instruction & 0x7;
   GPR[dest] = GPR[src1] & GPR[src2]; 
 }
 
 set_flag (dest);

}

void not (uint16_t instructions)
{
   uint16_t dest = (instruction >> 9) & 0x7;
   uint16_t src = (instruction >> 6) & 0x7;

   GPR[dest] = ~GPR[src];

   set_flag(dest);
}

/*
 * ======================
 * Trap Routines
 * ====================== 
 */

void trap_getc ()
{
  GPR[0] = (uint16_t) getchar();
  GPR[0] = GPR[0] & 0xff; // the high eight bits are to be cleared to not let the garbage affect the ASCII value of the entered char
  set_flag (0);
}

void trap_out ()
{
  putchar(GPR[0]);
}

void trap_puts ()
{
  uint16_t *c = memory + GPR[0];

  while (*c) // Print all characters from the first memory location pointed to by R0 (GPR[0]) until we hit the wall (\0)
  {
    putc ((char)*c, stdout);
	c++; 
  }
  fflush(stdout);
}

void trap_in ()
{
  printf ("Enter a character: ");
  trap_getc();
  trap_out();
  set_flag (0);
}

void trap_putsp()
{
  uint16_t *c = memory + GPR[0];
  uint16_t *c1 = *c & 0xFF;
  uint16_t *c2 = *c >> 8;

  while (*c) 
  {
    char c1 = *c & 0xFF;
    putc (c1, stdout);
    char c2 = *c >> 8;
	if (c2) putc (c2, stdout);
	c++; 
  }
  fflush(stdout);
}

void trap_halt()
{
  printf ("Halting the program... \n");
  running = 0;
}

