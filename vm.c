
// We shall keep the max memory to be 16-bit
#define MEMORY_CAPACITY (1 << 16)

uint16_t* memory = malloc (MEMORY_CAPACITY * sizeof (uint16_t));

/*
 * Registers:
 *    - There are 8 GPRs, 1 PC register, and 1 Conditional flag register
 *    - Each register is 16-bit long
 */
#define NUM_GPR 8 
unint16_t* GPR = malloc ( NUM_GPR * sizeof (unint16_t));

unint16_t* PC = malloc (sizeof (unint16_t));
unint16_t* COND = malloc (sizeof (unint16_t));

/*
 * OPCODES:
 *    - There are 16 OPCODES
 *    - Each of which is of 4-bits
 */

enum 
{
  OP_C0 = 0,
  OP_C1,
  OP_C2,
  OP_C3,
  OP_C4,
  OP_C5,
  OP_C6,
  OP_C7,
  OP_C8,
  OP_C9,
  OP_C10,
  OP_C11,
  OP_C12,
  OP_C13,
  OP_C14,
  OP_C15,
};

/*
 * Condition Flags
 *    - There are 3 condition flags
 */

enum 
{
  POS_F = 1 << 0;  // x x 1
  ZRO_F = 1 << 1;  // x 1 x
  NEG_F = 1 << 2;  // 1 x x
};














