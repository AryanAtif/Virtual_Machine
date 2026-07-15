#include <stdint.h>
#include <stdio.h>

#include "arch.h"
#include "instructions.h"

// See instruction.h for the description of each of the functions defined in this file

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


void read_image_file (FILE* file)
{
  
  uint16_t origin;
  fread (&origin, sizeof(origin), 1, file);
  to_big_endian (origin);

  uint16_t max_read = MAX_MEMORY - origin;
  uint16_t* p = memory + origin;
  fread (p, sizeof(p), max_read, file); 

  while (max_read-- > 0)
  {
    to_big_endian (*p);
    p++;
  }
}

int read_image(const char* image_path)
{
    FILE* file = fopen(image_path, "rb");
    if (!file) { return 0; };
    read_image_file(file);
    fclose(file);
    return 1;
}

uint16_t to_big_endian (uint16_t x)
{
  return (x << 8) | (x >> 8);
}

void mem_write (uint16_t address, uint16_t val)
{
  memory[address] = val;
}

uint16_t mem_read (uint16_t address)
{
  if (address == KBSR)
  {
    if (check_key())
    {
      memory[KBSR] = (1 << 15);
      memory[KBDR] = getchar();
    }
    else
    {
      memory[KBSR] = 0;
    }
  }
  return memory[address];
}



/*
 * =============
 * Instructions
 * =============
 */

void add (uint16_t instruction)
{
  uint16_t dest = (instruction >> 9) & 0x07;  // extract the dest register from the instruction
  uint16_t src1 = (instruction >> 6) & 0x07;  // extract the src1 register from the instruction
                                               
  bool is_imm = (instruction >> 5) & 1;                                        
  uint16_t src2;

  if (is_imm)
  { // if there's an immediate value
    src2 = instruction & 0x1F;
    src2 = sign_extend(src2, 5);
    GPR[dest] = GPR[src1] + src2; 
  }
  else
  {
    src2 = instruction & 0x7;
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

  pc_offset = sign_extend(pc_offset, 16);

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
    pc_offset = sign_extend(pc_offset, 9);

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

  mem_write (PC + pc_offset,  GPR[src1]);

  set_flag(src1);
}

void str (uint16_t instruction)
{
  uint16_t src1 = (instruction >> 9) & 0x7;
  uint16_t base_r = (instruction >> 6) & 0x7;
  uint16_t offset = instruction & 0x3F;

  offset = sign_extend(offset, 16);

  mem_write (GPR[base_r] + offset,  GPR[src1]);
  set_flag(src1);
}

void sti (uint16_t instruction)
{
  uint16_t src = (instruction >> 9) & 0x7;
  uint16_t pc_offset = instruction & 0x1FF; 

  pc_offset = sign_extend(pc_offset, 16);

  int mem_to_read = mem_read(PC + pc_offset);

  mem_write (mem_to_read,  GPR[src]); 

  set_flag(src);
}

void jsr (uint16_t instruction)
{
 bool is_label = (instruction >> 11) & 1;

 if (is_label)
 {
    uint16_t pc_offset = instruction & 0x7FF;
    pc_offset = sign_extend(pc_offset, 11);
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

void and (uint16_t instruction)
{
 uint16_t dest = (instruction >> 9) & 0x7;
 uint16_t src1 = (instruction >> 6) & 0x7;

 if ((instruction >> 5) & 1) // if there's an immediate value in the instruction
 {
   uint16_t imm = sign_extend (instruction & 0x1F, 5);
   GPR[dest] = GPR[src1] & imm; 
 }
 else
 {
   uint16_t src2 = instruction & 0x7;
   GPR[dest] = GPR[src1] & GPR[src2]; 
 }
 
 set_flag (dest);

}

void not (uint16_t instruction)
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

