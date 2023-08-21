#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cctype> //toupper
#include <unordered_map>
#include <algorithm> // For std::reverse
#include <bitset> // for decimal to bit conversion

class assembler
{

public:
    assembler();
    ~assembler();

    //structures and variables:
    std::map<std::string, std::string> symbolTable; //map of labels atributed to values

    std::map<std::string, std::string> lineAddressTable; //map of labels atributed to instructions (name and line number)

    struct Instruction { //structure for instructions with opcode for immeadiate acess and memory access
        std::string immediate;
        std::string memory;
    };

    std::unordered_map<std::string, Instruction> instructionMap; //map of instructions with their opcodes

    //Functions----------------------------------------------

    std::string toUpperCase(const std::string& input); //helper function - converts a string to uppercase

    void toUpperFile(const std::string& inputFilename); //helper function, converts whole file to uppercase

    std::string trim(const std::string& str); //helper function - removes spaces from a string

    std::string hexToBinary(const std::string& hex); //helper function - converts a hex string to binary

    std::string decimalToBinary(const std::string& decimalString); //helper function - converts a decimal string to binary
    
    std::string decimalToBinary2C(const std::string& decimalStr); //helper function - converts a decimal string to binary in 2's complement

    void preprocessor(std::ifstream &file); //prepares the program to be assembled

    void finalAssembly(std::ifstream &file, std::ofstream &out_file); //converts the prepared program to binary instructions
};