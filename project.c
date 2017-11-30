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
            case 000:
                *ALUresult = A + B;
                break;

        //001 - Z = A - B
            case 001:
                *ALUresult = A - B;
                break;

        //010 - If A < B, Z = 1; otherwise, Z = 0
        //Cast A and B to int.
            case 010:
                if((int)A < (int)B) {
                    *ALUresult = 1;
                }
                else {
                    *ALUresult = 0;
                }
                break;

        //011 - If A < B, Z = 1; otherwise, Z = 0
        //As unsigned integers.
            case 011:
                if(A < B) {
                    *ALUresult = 1;
                }
                else {
                    *ALUresult = 0;
                }
                break;

        //100 - Z = A AND B
            case 100:
                *ALUresult = (A & B);
                break;

        //101 - Z = A OR B
            case 101:
                *ALUresult = (A | B);
                break;

        //110 - Shift left B by 16 bits
            case 110:
                *ALUresult = B << 16;
                break;

        //111 - Z = NOT A
            case 111:
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
    // Kyle
    // find correct values that go in the controls
    switch (op) {
        case 0: // R type
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;

        case 8: //add immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        case 10: //slti
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        case 11: //sltiu
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        case 4: //Branch Equal
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;

        case 2: //Jump
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;

        case 35: //Load word
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        case 15: //load unsigned immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        case 43: //Store word
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;

        default:
            return 1;

    }
    return 0;

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
//Matt
    *data1 = Reg[r1];    //reads the value of the register at r1 and assigns it to data1
    *data2 = Reg[r2];    //reads the value of the register at r2 and assigns it to data2
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
//Matt
//Checks the sign bit and extends accordingly
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
    //Kyle

    //Check which data we are operating on by ALU src
    if (ALUSrc == 1) {
        data2 = extended_value;
    }

    //Instructions to ALU which updates ALU result
    //ALUOP 7 is R type instruction which requires use of funct
    if (ALUOp == 7) {
        switch (funct) {
                //Add
            case 32:
                ALUOp = 0;
                break;
                //Sub
            case 34:
                ALUOp = 1;
                break;
                //Set Less Signed
            case 42:
                ALUOp = 2;
                break;
                //Set Less Unsigned
            case 43:
                ALUOp = 3;
                break;
                //AND
            case 36:
                ALUOp = 4;
                break;
                //OR
            case 37:
                ALUOp = 5;
                break;
                //Shift Left
            case 6:
                ALUOp = 6;
                break;
                //NOR
            case 39:
                ALUOp = 7;
                break;
            default:
                return 1;
        }
        //Callback to ALU for funct
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    else {
        //Callback ALU for non funct
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{   //Kyle

    //reading from memory
    if (MemRead == 1) {
        if ((ALUresult % 4) == 0) {
            *memdata = Mem[ALUresult >> 2];
        }
            //Improper Address Halt
        else {
            return 1;
        }

    }

    //writing to memory
    if (MemWrite == 1) {
        if ((ALUresult % 4) == 0) {
            Mem[ALUresult >> 2] = data2;
        }
        else {
            return 1;
        }
    }

    return 0;
}




/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{   //Kyle

    //Check if writing
    if (RegWrite == 1) {
        // If Mem to Register
        if (MemtoReg == 1 && RegDst == 0) {
            Reg[r2] = memdata;
        }
            //If Mem to Register but r3

        else if (MemtoReg == 1 && RegDst == 1) {
            Reg[r3] = memdata;
        }
            // If Result to Register
        else if (MemtoReg == 0 && RegDst == 0) {
            Reg[r2] = ALUresult;
        }
            // If Result to Register but next value
        else if (MemtoReg == 0 && RegDst == 1) {
            Reg[r3] = ALUresult;
        }
    }

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
