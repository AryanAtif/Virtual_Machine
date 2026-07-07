
// We shall keep the max memory to be 16-bit
#define memory_capacity (1 << 16)

uint16_t* memory = malloc (memory_capacity * sizeof (uint16_t));

/*
 * Registers:
 *    - There are 8 GPRs, 1 PC register, and 1 Conditional flag register
 *    - Each register is 16-bit long
 */
#define NUM_GPR 8 
unint16_t* GPR = malloc ( NUM_GPR * sizeof (unint16_t));

unint16_t* PC = malloc (sizeof (unint16_t));
unint16_t* COND = malloc (sizeof (unint16_t));
