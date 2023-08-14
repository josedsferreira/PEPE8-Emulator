#include "PEPE8.h"
#include "assembler.h"
#include <array>
#include <iostream>
#include <string>
#include <fstream>

PEPE8::PEPE8() {

    std::ifstream program_file("binary_program.txt");

    if (!program_file.is_open()) {
        std::cerr << "Failed to open program file" << std::endl;
        return;	
    }
}

PEPE8::~PEPE8() {
}

void PEPE8::loadProgram(std::string filename) {

}
