#include <array>
#include <iostream>
#include <string>
#include <fstream>

class PEPE8 {

public:
    PEPE8();
    ~PEPE8();

    //Functions:
    void loadProgram();

    //2 memories (teoretically they exist outside the processor):
    std::array<uint8_t, 1 * 1024> data; 
    //1KB not sure how much i need

    std::array<std::string, 100> instructions; 
    // 1KB positions in the array (again not sure how much needed). 2 bytes per instruction
    //Program is loadded in this memory one line/instruction per position (?)

    //Variables:
    uint8_t A; //Accumulator
    
};