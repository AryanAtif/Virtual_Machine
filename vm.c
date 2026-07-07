
// We shall keep the max memory to be 16-bit
#define memory_capacity (1 << 16)

uint16_t* memory = malloc (sizeof (memory_capacity * uint16_t));
