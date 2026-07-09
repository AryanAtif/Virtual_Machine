#include <stdint.h>

#include "vm.h"

// See vm.h for the description of each of the functions defined in this file

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

  set_flag[dest];
}
