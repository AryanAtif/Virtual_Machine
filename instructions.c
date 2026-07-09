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
