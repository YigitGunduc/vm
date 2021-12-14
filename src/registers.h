#ifndef REGISTERS_H_
#define REGISTERS_H_

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

#endif // REGISTERS_H_
