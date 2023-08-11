#include "PEPE8.h"
#include <array>
#include <iostream>
#include <string>
#include <fstream>

PEPE8::PEPE8() {
}

PEPE8::~PEPE8() {
}

void PEPE8::loadProgram() {

    //passar isto para o objeto compiler

    std::ifstream file;

    file.open("program.txt");

    if (!file) {
        std::cout << "Unable to open file";
    }
    else {
        std::cout << "File opened successfully";
        
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            instructions[i] = line;
            i++;
        }
        
        file.close();
    }
}