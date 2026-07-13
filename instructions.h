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
void add (uint16_t instruction);                                    /* Add src1 and src2 and store the result in dest. if the instruction contains an 
                                                                     * immediate value, it will be store in the variable src2, not the register
                                                                     * element src2
                                                                     */

void ldi (uint16_t instruction);                                 /* read the value stored in the address pointed by PC + PCOFFSET9 into dest register */

void ld (uint16_t instruction);                                  /* read the value stored in PC + PCOFFSET9 into dest register */

void ldr (uint16_t instruction);                                /* read the value stored in the base register + OFFSE9 into dest register */

void lea (uint16_t instruction);                                /* read the effective address: PC + PC_OFFSET9 into dest register */

void br (uint16_t instruction);                                 /* Branch out if the flag given in the instruction is set in the program*/

void st (uint16_t instruction);                                 /* store the value to PC + PCOFFSET9 from src1 */  

void str (uint16_t instruction);                                 /* store the value to Base register + PCOFFSET9 from src1 */  

void sti (uint16_t instruction);                                 /* store the value to the address pointed by PC + PCOFFSET9 from src */  

void jsr (uint16_t instruction);                                 /* Jump to subroutine. if given a pcoffset of 11-bits, the PC will be modified to 
                                                                  * PC = PC + PC_OFFSET11. but if given a base register of 3-bits, the PC will be 
                                                                  * modified to PC = base_register 
                                                                  */

void jmp (uint16_t instruction);                                  /* Jump to subroutine given by the base register */

void and (uint16_t instruction);                                 /* store the logical AND result of src1 and src2 registers (or an immediate value,
                                                                  * instead of the src2 register) inside the dest register
                                                                  */
       
void not (uint16_t instruction);                                 /* store the logical NOT of the src register in the dest*/


/* Trap Routines */

void trap_getc();                                               /* Input a char into R0 without echoing it to the terminal */

void trap_out();                                                /* Output the character stored in R0 to the terminal */

void trap_puts();                                               /* Output an ASCII string stored in the memory location */

void trap_in();                                               /* Read a char into R0, echo it to the terminal */

#endif

