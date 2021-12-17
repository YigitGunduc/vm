
# VM

A simple virtual machine build with C
## Getting Started

### Instructions
```c

enum {
  MOVE_LIT_TO_REG = 0x10,
  MOVE_REG_TO_REG,
  MOVE_REG_TO_MEM,
  MOVE_MEM_TO_REG,
  MOVE_LIT_TO_MEM,
  ADD_REG_TO_REG,
  ADD_LIT_TO_REG,
  SUB_LIT_TO_REG,
  SUB_REG_TO_REG,
  INC_ACC,
  DEC_ACC,
  INC_REG,
  DEC_REG,
  MOVE_MMEM_TO_REG,
  MOVE_REG_TO_MMEM,
  MOVE_LIT_TO_MMEM,
  JMP_NOT_EQ,
  PSH_LIT,
  PSH_REG,
  POP,
  CAL_LIT,
  CAL_REG,
  RET,
  HLT,
};
```

### Registers
```c
enum 
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_ACC, /* accumulator */
    R_IP, /* instruction pointer */
    R_SP, /* stack pointer */
    R_FP,
    R_COUNT /* number of registers */
};
```

### Stack
functions conserning Stack
```c
// function to create a stack of given capacity
struct Stack* createStack(unsigned int capacity);

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack);
 
// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack);
 
// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, int item);
 
// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack *stack);
 
// Function to return the top from stack without removing it
int peek(struct Stack *stack);
```
Instructions conserning stack
```c
PSH_LIT,
PSH_REG,
POP,
```

### A simple program
```
cpu->memory[i++] = MOVE_LIT_TO_REG; 
cpu->memory[i++] = 0xAB;
cpu->memory[i++] = R_R1;

cpu->memory[i++] = MOVE_REG_TO_REG;
cpu->memory[i++] = R_R1;
cpu->memory[i++] = R_R2;

cpu->memory[i++] = ADD_REG_TO_REG;
cpu->memory[i++] = R_R1;
cpu->memory[i++] = R_R2;

cpu->memory[i++] = MOVE_REG_TO_MEM;
cpu->memory[i++] = R_ACC;
cpu->memory[i++] = 5;

cpu->memory[i++] = HLT;
```

### Runing The VM
```bash
bash build.sh
```

### Maped memory

```c
char *map_memory_for_read(const char *file_name);
char *map_memory_for_write(const char *file_name, size_t file_size);

MOVE_MMEM_TO_REG,
MOVE_REG_TO_MMEM,
MOVE_LIT_TO_MMEM,
```

## License

[MIT](https://choosealicense.com/licenses/mit/)
