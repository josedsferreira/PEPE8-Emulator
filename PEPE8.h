#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

class PEPE8 {

public:
    PEPE8();
    ~PEPE8();

    //Functions:
    void loadProgram();
    void clock(); //Runs the processor and the program

    //2 memories (teoretically they exist outside the processor):
    std::array<int8_t, 1 * 1024> data; 
    //1KB not sure how much i need or how much there is in the real processor
    std::array<uint16_t, 255> instructions; 
    // 255 positions in the array (number of addresses allowed in 8bits). 2 bytes per instruction
    //Program is loadded in this memory one line/instruction per position

    //Registers:
    uint8_t PC = 0; //Program Counter
    int8_t A; //Accumulator
    uint8_t SEL_PC;
    uint8_t SEL_ALU;
    bool ESCR_A;
    bool SEL_A;
    bool SEL_B;
    bool WR;

    //Helper variables:
    uint16_t instructions_end = 0xFFFF; //signal end of the positions used of the array
    uint8_t instruction; //current instruction
    int8_t operand; //current operand

    //Assembly instruction functions:
    void LD(int8_t operand); //Load register A with operand
    void LD_m(int8_t operand); //Load register A with value in memory address operand
    void ST(int8_t operand); //Store value in register A in memory address operand
    void ADD(int8_t operand); //Add operand to register A
    void ADD_m(int8_t operand); //Add value in memory address operand to register A
    void SUB(int8_t operand); //Subtract operand from register A
    void SUB_m(int8_t operand); //Subtract value in memory address operand from register A
    void AND(int8_t operand); //Logical AND operand with register A
    void AND_m(int8_t operand); //Logical AND value in memory address operand with register A
    void OR(int8_t operand); //Logical OR operand with register A
    void OR_m(int8_t operand); //Logical OR value in memory address operand with register A
    void JMP(int8_t operand); //Jump to instruction with memory address operand
    void JZ(int8_t operand); //Jump to instruction with memory address operand if register A is zero
    void JN(int8_t operand); //Jump to instruction with memory address operand if register A is negative
    
    
};