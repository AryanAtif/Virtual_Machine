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

void add (uint16_t dest, uint16_t src1, bool is_imm, uint16_t src2)
{

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

  dest = mem_read (mem_to_read); // mem_to_read is itself an address to a value

  set_flag(dest);
}

void br (uint16_t instruction)
{
  uint16_t flag_given = (instruction >> 9) & 0x7;
  // 100 = n
  // 010 = p
  // 001 = z
  // 000 = none, return

  bool is_flag_set = ((flag_given == 0b100) & NEG_F) || ((flag_given == 0b010) & POS_F) || ((flag_given == 0b001) & ZRO_F);

  if (is_flag_set)
  {
    uint16_t pc_offset = instruction & 0x1FF;
    pc_offset = sign_extend(pc_offset, 16);

    PC = pc_offset;
  }
}


