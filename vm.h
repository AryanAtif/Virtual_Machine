#ifndef VM_H 
#define VM_H

#include <stdint.h>

/* Helper Functions */

uint16_t sign_extend(uint16_t x, int bit_count);  /* 
                                                   * will be used for sign extension (extending  the 5-bit x with 0's or 1's depending on its sign to
                                                   * make it 16-bit, if required by the instruction)
                                                   */

void set_flag (uint16_t reg);                     /* Set the condition flag up based on the sign of the value in the reg */

/* Functions for Instructions */
  
 

#endif

