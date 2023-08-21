#include "PEPE8.h"

PEPE8::PEPE8() {

    decoderMap.insert({ "00000000", "LD" });
    decoderMap.insert({ "00000001", "LD" });
    decoderMap.insert({ "00000010", "ST" });
    decoderMap.insert({ "00000011", "ADD" });
    decoderMap.insert({ "00000100", "ADD" });
    decoderMap.insert({ "00000101", "SUB" });
    decoderMap.insert({ "00000110", "SUB" });
    decoderMap.insert({ "00000111", "AND" });
    decoderMap.insert({ "00001000", "AND" });
    decoderMap.insert({ "00001001", "OR" });
    decoderMap.insert({ "00001010", "OR" });
    decoderMap.insert({ "00001011", "JMP" });
    decoderMap.insert({ "00001100", "JZ" });
    decoderMap.insert({ "00001101", "JN" });
    decoderMap.insert({ "00001110", "NOP" });
    
    /*Debugging
    std::cout << "Reg A: " << std::to_string(A) << std::endl;
    std::cout << "SEL_PC: " << std::bitset<4>(SEL_PC) << std::endl;
    std::cout << "SEL_ALU: " << std::bitset<4>(SEL_ALU) << std::endl;
    std::cout << "ESCR_A: " << ESCR_A << std::endl;
    std::cout << "SEL_A: " << SEL_A << std::endl;
    std::cout << "SEL_B: " << SEL_B << std::endl;
    std::cout << "WR: " << ESCR_A << std::endl;
    */
}

PEPE8::~PEPE8() {
}

void PEPE8::loadProgram() {

    std::string line;
    int i = 0;
    int j = 0;

    std::ifstream binary_program_file("binary_program.txt");

    if (!binary_program_file.is_open()) {
        std::cerr << "Failed to open binary program file" << std::endl;
        return;	
    }

    while (std::getline(binary_program_file, line) && i < instructions.size()) {
        try {
            decoder(line, i); // Decode the instruction and operand into a string for the gui
            uint16_t number = std::bitset<16>(line).to_ulong();
            instructions[i] = number; // Store the instruction + operand in the array
            i++;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line when loading program: " << e.what() << std::endl;
        }
    }

    if (i < instructions.size()) {
        instructions[i] = instructions_end; // signal end of the positions used of the array
        decoded_instructions[i] = "END";
    }    

    binary_program_file.close();

    /*//Debugging
    //Print the contents of the arrays for debugging
    std::cout << "Contents of instructions memory:" << std::endl;

    for (j = 0; j <= i; ++j) {
        std::cout << "instructions[" << j << "] = " << std::bitset<16>(instructions[j]) << std::endl;
    }

    
    std::cout << "Contents of instructions gui map:" << std::endl;

    for (j = 0; j <= i; ++j) {
        std::cout << "instructions[" << j << "] = " << decoded_instructions[j] << std::endl;
    }
    */
    
}

void PEPE8::checkProgram() {

    std::cout << "DEBBUGING PROGRAM:" << std::endl;

    /*
    for (int j = 0; instructions[j] != instructions_end; ++j) {
        std::cout << "instructions[" << j << "] = " << std::bitset<16>(instructions[j]) << std::endl;
    }
    */
    
    while (PC < instructions.size() && instructions[PC] != instructions_end)
    {
        std::cout << "instructions[" << PC << "] = " << std::bitset<16>(instructions[PC]) << std::endl;
        PC++;
    }
    PC = 0;

}

void PEPE8::decoder(std::string line, int i) {
    
        std::string opcode = line.substr(0, 8);
        std::string operand = line.substr(8, 8);
        std::string decoded_opcode;
        std::string decoded_operand;

        if (decoderMap.find(opcode) != decoderMap.end()) {
            decoded_opcode = decoderMap[opcode];
        }
        else {
            throw std::invalid_argument("Invalid opcode IN DECODER");
        }

        /*
        // Debugging
        std::cout << "OPCODE:" << opcode << std::endl; //debug opcode
        std::cout << "DECODED OPCODE:" << decoded_opcode << std::endl; //debug opcode
        std::cout << "OPERAND:" << std::to_string(std::bitset<8>(operand).to_ulong()) << std::endl; //debug operand
        */

        switch (std::bitset<8>(opcode).to_ulong()) {
            case 0:
            case 1:
            case 3:
            case 5:
            case 7:
            case 9:
            case 11:
            case 12:
            case 13:
                // immediate value
                decoded_operand = std::to_string(std::bitset<8>(operand).to_ulong()); //convert to decimal, always as string
                break;
            case 2:
            case 4:
            case 6:
            case 8:
            case 10:
                // memory address
                decoded_operand = "[" + std::to_string(std::bitset<8>(operand).to_ulong()) + "]"; //convert to hex, always as string
                break;
            case 14:
                decoded_operand = "";
                break;
        }
        
        //std::cout << "FULL INSTRUCTION: " << decoded_opcode + " " + decoded_operand << std::endl; //debug operand
        decoded_instructions[i] = std::to_string(i) + ": " + decoded_opcode + " " + decoded_operand;
}

void PEPE8::clock() {

    if (PC < instructions.size() && instructions[PC] != instructions_end)
    {

        instruction = (instructions[PC] >> 8) & 0xFF;
        operand = instructions[PC] & 0xFF;

        /*
        //debugging
        std::cout << "line: " << std::to_string(instructions[PC]) << std::endl << std::flush;
        std::cout << "instruction: " << std::to_string(instruction) << std::endl << std::flush;
        std::cout << "operand: " << std::to_string(operand) << std::endl << std::flush;
        */

        switch (instruction) {
            case 0:
                LD(operand);
                break;
            case 1:
                LD_m(operand);
                break;
            case 2:
                ST(operand);
                break;
            case 3:
                ADD(operand);
                break;
            case 4:
                ADD_m(operand);
                break;
            case 5:
                SUB(operand);
                break;
            case 6:
                SUB_m(operand);
                break;
            case 7:
                AND(operand);
                break;
            case 8:
                AND_m(operand);
                break;
            case 9:
                OR(operand);
                break;
            case 0xA:
                OR_m(operand);
                break;
            case 0xB:
                if (operand == PC)
                {
                    std::cout << "HALT - Program execution completed!" << std::endl;
                    haltFlag = true;
                    return;
                }
                JMP(operand);
                break;
            case 0xC:
                JZ(operand);
                break;
            case 0xD:
                JN(operand);
                break;
            case 0xE:
                //NOP no operation
                break;
        }
    }
    else {
        std::cout << "ERROR - PC may be out of bounds" << std::endl;
        return;
    }

    /*
    //Debugging
    std::cout << "PC: " << std::to_string(PC) << std::endl;
    std::cout << "Reg A: " << std::to_string(A) << std::endl;
    std::cout << "SEL_PC: " << std::bitset<4>(SEL_PC) << std::endl;
    std::cout << "SEL_ALU: " << std::bitset<4>(SEL_ALU) << std::endl;
    std::cout << "ESCR_A: " << ESCR_A << std::endl;
    std::cout << "SEL_A: " << SEL_A << std::endl;
    std::cout << "SEL_B: " << SEL_B << std::endl;
    std::cout << "WR: " << ESCR_A << std::endl;
    std::cout << "" << std::endl;
    */

    PC++;

}

/*Old continous running clock
void PEPE8::clock() {

    std::cout << "Starting clock" << std::endl;

    for (PC = 0; PC < instructions.size() && instructions[PC] != instructions_end; PC++)
    {
        instruction = instructions[PC] >> 8;
        operand = instructions[PC] & 0xFF;

        // Debugging
        //std::cout << "instructions[" << std::to_string(PC) << "] = Instruction: " 
        //<< std::bitset<8>(instruction) << " operand: " << std::bitset<8>(operand) << std::endl;
        

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
*/

void PEPE8::LD(int8_t operand) {
    A = operand;
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = true;
    SEL_A = true; //MUX_B is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
    //std::cout << "LD instruction performed" << std::endl; //debugging
}

void PEPE8::LD_m(int8_t operand) {
    A = data[operand];
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = true;
    SEL_A = true; //MUX_B is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
    //std::cout << "LD instruction performed" << std::endl; //debugging
}

void PEPE8::ST(int8_t operand) {
    data[operand] = A;
    SEL_PC = 0; //no jump
    //SEL_ALU = XX //no ALU operation
    ESCR_A = false; //no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = true; // write to memory
    //std::cout << "ST instruction performed" << std::endl; //debugging
}

void PEPE8::ADD(int8_t operand) {
    A += operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 0; //add
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
    //std::cout << "ADD instruction performed" << std::endl; //debugging
}

void PEPE8::ADD_m(int8_t operand) {
    A += data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 0; //add
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
    //std::cout << "ADD instruction performed" << std::endl; //debugging
}

void PEPE8::SUB(int8_t operand) {
    A -= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 1; //subtract
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
    //std::cout << "SUB instruction performed" << std::endl; //debugging
}

void PEPE8::SUB_m(int8_t operand) {
    A -= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 1; //subtract
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
    //std::cout << "SUB instruction performed" << std::endl; //debugging
}

void PEPE8::AND(int8_t operand) {
    A &= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
    //std::cout << "AND instruction performed" << std::endl; //debugging
}

void PEPE8::AND_m(int8_t operand) {
    A &= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
    //std::cout << "AND instruction performed" << std::endl; //debugging
}

void PEPE8::OR(int8_t operand) {
    A |= operand;
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = false; //operand is the source
    WR = false; // no write to memory
    //std::cout << "OR instruction performed" << std::endl; //debugging
}

void PEPE8::OR_m(int8_t operand) {
    A |= data[operand];
    SEL_PC = 0; //no jump
    SEL_ALU = 2; //and
    ESCR_A = true; //write to A
    SEL_A = false; //ALU is the source
    SEL_B = true; //memory is the source
    WR = false; // no write to memory
    //std::cout << "OR instruction performed" << std::endl; //debugging
}

void PEPE8::JMP(int8_t operand) {
    PC = operand-1;
    SEL_PC = 1; //jump
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
    //std::cout << "JMP instruction performed" << std::endl; //debugging
}

void PEPE8::JZ(int8_t operand) {
    if (A == 0) {
        PC = operand-1;
    }
    SEL_PC = 2; //jump if A is zero
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
    //std::cout << "JZ instruction performed" << std::endl; //debugging
}

void PEPE8::JN(int8_t operand) {
    if (A < 0) {
        PC = operand-1;
    }
    SEL_PC = 3; //jump if A negative
    //SEL_ALU = XX; //no effect
    ESCR_A = false; // no write to A
    //SEL_A = X; //no effect
    //SEL_B = X; //no effect
    WR = false; // no write to memory
    //std::cout << "JN instruction performed" << std::endl; //debugging
}

