#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include "assembler.h"

class PEPE8 {

public:
    PEPE8();
    ~PEPE8();

    //Functions:
    void loadProgram(std::string filename);

    //2 memories (teoretically they exist outside the processor):
    std::array<uint8_t, 1 * 1024> data; 
    //1KB not sure how much i need or how much there is in the real processor

    std::array<std::string, 255> instructions; 
    // 255 positions in the array (number of addresses allowed in 8bits). 2 bytes per instruction
    //Program is loadded in this memory one line/instruction per position

    //Variables:
    uint8_t A; //Accumulator
    
};