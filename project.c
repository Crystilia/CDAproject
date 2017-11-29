#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
//Sarah

    //Implement the operations on input parameters A and B according to ALUControl.
    //Output the result (Z) to ALUresult.

    switch(ALUControl) {

        //000 - Z = A + B
            case 0:
                *ALUresult = A + B;
                break;

        //001 - Z = A - B
            case 1:
                *ALUresult = A - B;
                break;

        //010 - If A < B, Z = 1; otherwise, Z = 0
        //Cast A and B to int.
            case 2:
                if((int)A < (int)B) {
                    *ALUresult = 1;
                }
                else {
                    *ALUresult = 0;
                }
                break;

        //011 - If A < B, Z = 1; otherwise, Z = 0
        //As unsigned integers.
            case 3:
                if(A < B) {
                    *ALUresult = 1;
                }
                else {
                    *ALUresult = 0;
                }
                break;

        //100 - Z = A AND B
            case 4:
                *ALUresult = (A & B);
                break;

        //101 - Z = A OR B
            case 5:
                *ALUresult = (A | B);
                break;

        //110 - Shift left B by 16 bits
            case 6:
                *ALUresult = B << 16;
                break;

        //111 - Z = NOT A
            case 7:
                *ALUresult = ~A;
                break;
        }

    //Assign Zero to 1 if the result is zero; otherwise, assign 0.
        if(*ALUresult == 0) {
            *Zero = 1;
        }
        else {
            *Zero = 0;
        }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
//Sarah

    //Fetch the instruction addressed by PC from Mem and write it to instruction.
	   *instruction = Mem[PC >> 2];

    //Return 1 if a halt condition occurs; otherwise, return 0.
        if(PC % 4 != 0) {
	      return 1;
      }
        else {
	      return 0;
      }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
//Sarah

    //Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
    //Shifting bits and bit masking to get only important parts from instruction.

    //Instruction [31-26]
        *op = (instruction >> 26) & 0x0000003f;

    //Instruction [25-21]
        *r1 = (instruction >> 21) & 0x0000001F;

    //Instruction [20-16]
        *r2 = (instruction >> 16) & 0x0000001F;

    //Instruction [15-11]
        *r3 = (instruction >> 11) & 0x0000001F;

    //Instruction [5-0]
        *funct = instruction & 0x0000003F;

    //Instruction [15-0]
        *offset = instruction & 0X0000FFFF;

    //Instruction [25-0]
        *jsec = instruction & 0x03FFFFFF;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
//Sarah - TODO
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
//Matt
    //Not sure if this is how this is supposed to be done.
    data1 = Reg[r1];    //reads the value of the register at r1 and assigns it to data1
    data2 = Reg[r2];    //reads the value of the register at r2 and assigns it to data2
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
//Matt
    unsigned mask = 0x8000;
    if ((offset & mask) == 0x8000)
    {
      *extended_value |= 0xFFFF0000;
    }
    *extended_value |= offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
//Matt
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
//Sarah

    //Update the program counter (PC).

    //Increment by 4 to keep from halting.
        *PC += 4;

    //Check to see if branching and Zero.
    //If true, add extended_value bitshifted left 2. If false, check to see if jumping.
    //If jumping is true, set to jump register and shift left 2.
        if(Branch == 1 && Zero) {
            *PC += extended_value << 2;
        }
        else if(Jump == 1) {
            *PC = (jsec << 2) | (*PC | 0xf0000000);
        }
}

