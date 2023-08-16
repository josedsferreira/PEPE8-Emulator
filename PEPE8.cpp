#include "PEPE8.h"

PEPE8::PEPE8() {

    PEPE8::loadProgram();
    PEPE8::clock();

    std::cout << "Reg A: " << std::to_string(A) << std::endl;
    std::cout << "SEL_PC: " << std::bitset<4>(SEL_PC) << std::endl;
    std::cout << "SEL_ALU: " << std::bitset<4>(SEL_ALU) << std::endl;
    std::cout << "ESCR_A: " << ESCR_A << std::endl;
    std::cout << "SEL_A: " << SEL_A << std::endl;
    std::cout << "SEL_B: " << SEL_B << std::endl;
    std::cout << "WR: " << ESCR_A << std::endl;
}

PEPE8::~PEPE8() {
}

void PEPE8::loadProgram() {

    std::string line;
    size_t i = 0;

    std::ifstream binary_program_file("binary_program.txt");

    if (!binary_program_file.is_open()) {
        std::cerr << "Failed to open binary program file" << std::endl;
        return;	
    }

    while (std::getline(binary_program_file, line) && i < instructions.size()) {
        try {
            uint16_t number = std::bitset<16>(line).to_ulong();
            instructions[i] = number; // Store the number in the array
            i++;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line when loading program: " << e.what() << std::endl;
        }
    }

    if (i < instructions.size()) {
        instructions[i] = instructions_end; // signal end of the positions used of the array
    }    

    binary_program_file.close();

    // Print the contents of the array for debugging
    for (size_t j = 0; j < i; ++j) {
        std::cout << "instructions[" << j << "] = " << std::bitset<16>(instructions[j]) << std::endl;
    }
}

void PEPE8::clock() {

    std::cout << "Starting clock" << std::endl;

    for (PC = 0; PC < instructions.size() && instructions[PC] != instructions_end; PC++)
    {
        instruction = instructions[PC] >> 8;
        operand = instructions[PC] & 0xFF;

        /* Debugging
        std::cout << "instructions[" << std::to_string(PC) << "] = Instruction: " 
        << std::bitset<8>(instruction) << " operand: " << std::bitset<8>(operand) << std::endl;
        */

        switch (instruction) {
            case 0:
                PEPE8::LD(operand);
                break;
            case 1:
                PEPE8::LD_m(operand);
                break;
            case 2:
                PEPE8::ST(operand);
                break;
            case 3:
                PEPE8::ADD(operand);
                break;
            case 4:
                PEPE8::ADD_m(operand);
                break;
            case 5:
                PEPE8::SUB(operand);
                break;
            case 6:
                PEPE8::SUB_m(operand);
                break;
            case 7:
                PEPE8::AND(operand);
                break;
            case 8:
                PEPE8::AND_m(operand);
                break;
            case 9:
                PEPE8::OR(operand);
                break;
            case 0xA:
                PEPE8::OR_m(operand);
                break;
            case 0xB:
                if (operand == PC)
                {
                    std::cout << "HALT - Program execution completed!" << std::endl;
                    return;
                }
                PEPE8::JMP(operand);
                break;
            case 0xC:
                PEPE8::JZ(operand);
                break;
            case 0xD:
                PEPE8::JN(operand);
                break;
            case 0xE:
                //NOP no operation
                break;
        }
    }
}

void PEPE8::LD(int8_t operand) {
    A = operand;
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = true;
    SEL_A = true; //MUX_B is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
}

void PEPE8::LD_m(int8_t operand) {
    A = data[operand];
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = true;
    SEL_A = true; //MUX_B is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
}

void PEPE8::ST(int8_t operand) {
    data[operand] = A;
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = false; //no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = true; // write to memory
}

void PEPE8::ADD(int8_t operand) {
    A += operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 0; //add
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
}

void PEPE8::ADD_m(int8_t operand) {
    A += data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 0; //add
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
}

void PEPE8::SUB(int8_t operand) {
    A -= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 1; //subtract
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
}

void PEPE8::SUB_m(int8_t operand) {
    A -= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 1; //subtract
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
}

void PEPE8::AND(int8_t operand) {
    A &= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
}

void PEPE8::AND_m(int8_t operand) {
    A &= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
}

void PEPE8::OR(int8_t operand) {
    A |= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
}

void PEPE8::OR_m(int8_t operand) {
    A |= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
}

void PEPE8::JMP(int8_t operand) {
    PC = operand;
    PC--;
    SEL_PC = 1; //jump
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
}

void PEPE8::JZ(int8_t operand) {
    if (A == 0) {
        PC = operand;
        PC--;
    }
    SEL_PC = 2; //jump if A is zero
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
}

void PEPE8::JN(int8_t operand) {
    if (A < 0) {
        PC = operand;
        PC--;
    }
    SEL_PC = 3; //jump if A negative
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
}

