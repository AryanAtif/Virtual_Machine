#ifndef INSTRUCTIONS_H 
#define INSTRUCTIONS_H

#include <stdint.h>

/* Helper Functions */

uint16_t sign_extend(uint16_t x, int bit_count);  /* 
                                                   * will be used for sign extension (extending  the 5-bit x with 0's or 1's depending on its sign to
                                                   * make it 16-bit, if required by the instruction)
                                                   */

void set_flag (uint16_t reg);                     /* Set the condition flag up based on the sign of the value in the reg */

/* Functions for Instructions */
void add (uint16_t dest, uint16_t src1, bool is_imm, uint16_t src2); /* Add src1 and src2 and store the result in dest. if the instruction contains an 
                                                                     * immediate value, it will be store in the variable src2, not the register
                                                                     * element src2
                                                                     */

void ldi (uint16_t instruction);                                 /* read the value stored in the address pointed by PC + PCOFFSET9 into dest register */

void ld (uint16_t instruction);                                  /* read the value stored in PC + PCOFFSET9 into dest register */

void br (uint16_t instruction);                                 /* Branch out if the flag given in the instruction is set in the program*/

void st (uint16_t instruction);                                 /* store the value to PC + PCOFFSET9 from src1 */  

void jsr (uint16_t instruction);                                 /* Jump to subroutine. if given a pcoffset of 11-bits, the PC will be modified to 
                                                                  * PC = PC + PC_OFFSET11. but if given a base register of 3-bits, the PC will be 
                                                                  * modified to PC = base_register 
                                                                  * */  

void and (uint16_t instruction);                                 /* store the logical AND result of src1 and src2 registers (or an immediate value,
                                                                  * instead of the src2 register) inside the dest register
                                                                  */

#endif

