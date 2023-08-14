#include "assembler.h"

assembler::assembler()
{
    std::cout << "assembler starting" << std::endl;

    // Initialize instruction map
    instructionMap["LD"]    = {"00000000", "00000001"};
    instructionMap["ST"]    = {"00001111", "00000010"};
    instructionMap["ADD"]   = {"00000011", "00000100"};
    instructionMap["SUB"]   = {"00000101", "00000110"};
    instructionMap["AND"]   = {"00000111", "00001000"};
    instructionMap["OR"]    = {"00001001", "00001010"};
    instructionMap["JMP"]   = {"00001011", "00001111"};
    instructionMap["JZ"]    = {"00001100", "00001111"};
    instructionMap["JN"]    = {"00001101", "00001111"};
    instructionMap["NOP"]   = {"00001110", "00001110"};

    std::ifstream program_file("assembly_program.txt");

    if (!program_file.is_open()) {
        std::cerr << "Failed to open program file" << std::endl;
        return;	
    }

    assembler::preprocessor(program_file); // Perform pre-processing step
                                           // creates a symbol table and line address table and converts the labels
                                           // removes comments, blank lines, spaces, and labels
                                           // converts the program to uppercase

    std::cout << "labels" << std::endl; //debugging
    for (const auto& entry : symbolTable) {
        std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
    }
    std::cout << "Line adresses" << std::endl; //debugging
    for (const auto& entry : lineAddressTable) {
        std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
    }

    std::cout << "Starting final assembly " << std::endl;

    std::ofstream cleanfile("binary_program.txt", std::ofstream::out | std::ofstream::trunc); //erase contents of binary program file
    cleanfile.close();

    std::ifstream processed_file2("processed_program.txt");
    std::ofstream binary_file("binary_program.txt");

    if (!binary_file.is_open() || !processed_file2.is_open()) {
        std::cerr << "Failed to open files!" << std::endl;
        return;	
    }

    assembler::finalAssembly(processed_file2, binary_file); // Perform final assembly step
                                                            // converts the program to binary instructions
    
    processed_file2.close();
    binary_file.close();
    
}

assembler::~assembler()
{
}

std::string assembler::toUpperCase(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c = std::toupper(c);
    }
    return result;
}

std::string assembler::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

void assembler::toUpperFile(const std::string& inputFilename) {
    std::ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFilename << std::endl;
        return;
    }

    std::ofstream outFile("temp.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << "temp.txt" << std::endl;
        inFile.close();
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        for (char& c : line) {
            c = std::toupper(c); // Convert each character to uppercase
        }
        outFile << line << std::endl; // Write modified line to the output file
    }

    inFile.close();
    outFile.close();

    //write the temp file to the original file

    std::ifstream inFile2("temp.txt");
    if (!inFile2.is_open()) {
        std::cerr << "Failed to open input file: " << "temp.txt" << std::endl;
        return;
    }

    std::ofstream outFile2(inputFilename);
    if (!outFile2.is_open()) {
        std::cerr << "Failed to open output file: " << inputFilename << std::endl;
        inFile.close();
        return;
    }

    while (std::getline(inFile2, line)) {
        outFile2 << line << std::endl; // Write temp file line to the output file
    }

    inFile2.close();
    outFile2.close();

    std::ofstream temp("temp.txt", std::ofstream::out | std::ofstream::trunc); //erase contents of temp file
    temp.close();
}

std::string assembler::hexToBinary(const std::string& hex) {
    std::map<char, std::string> hexToBinMap = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    std::string binary;

    for (char c : hex) {
        if (hexToBinMap.find(c) != hexToBinMap.end()) {
            binary += hexToBinMap[c];
        } else {
            std::cerr << "Invalid hexadecimal digit: " << c << std::endl;
            return "";
        }
    }

    return binary;
}

std::string assembler::decimalToBinary(const std::string& decimalStr) {
    // Convert the decimal string to an integer
    int decimalValue = std::stoi(decimalStr);

    // Convert the integer to binary string
    std::string binaryStr = std::bitset<8>(decimalValue).to_string();

    // Make sure the binary string has exactly 8 bits
    if (binaryStr.length() > 8) {
        std::cerr << "Invalid number: " << binaryStr << " Cant be more than 8 bits" << std::endl;
    } else if (binaryStr.length() < 8) {
        binaryStr = std::string(8 - binaryStr.length(), '0') + binaryStr;
    }

    return binaryStr;
}

void assembler::preprocessor(std::ifstream &in_file)
{
    std::string line;
    int lineNumber = 0;
    bool startFound = false;

    std::ofstream out_file("processed_program.txt");
    if (!out_file.is_open()) {
        std::cerr << "Failed to open processed program file at preprocessing start!" << std::endl;
        return;	
    }

    // First pass: find all value labels and store them in the symbol table
    while (std::getline(in_file, line)) {
        std::string uncommentedLine = line.substr(0, line.find(';')); // Remove comments
        std::istringstream iss(uncommentedLine);
        std::string label;
        std::string instruction;
        std::string operand;

        if (iss >> label >> instruction >> operand) {
            if (instruction == "EQU")
            {
                symbolTable[assembler::trim(label)] = assembler::trim(operand);
            }
        }
    }

    in_file.clear();
    in_file.seekg(0); // Reset file to beginning

    std::cout << "first pass complete" << std::endl;
    //second pass: find all labels and exchange them for their values

    while (std::getline(in_file, line)) {
        std::string uncommentedLine = line.substr(0, line.find(';')); // Remove comments
        if (uncommentedLine.empty()) continue; // Skip empty lines
        std::istringstream iss(uncommentedLine);
        std::string token;
        std::string processedLine; // to store the processed line

        if (startFound == true) { //case where the program has started already
            
            while (iss >> token) {
                if (token.front() == '[' && token.back() == ']') {
                    // Extract word inside brackets
                    std::string wordInsideBrackets = token.substr(1, token.length() - 2);
                    
                    if (symbolTable.find(wordInsideBrackets) != symbolTable.end()) {
                        processedLine += "[" + symbolTable[wordInsideBrackets] + "] "; // Add brackets back and change the word inside
                    } else {
                        processedLine += token + " ";
                    }
                } else {
                    if (symbolTable.find(assembler::trim(token)) != symbolTable.end()) {
                        processedLine += symbolTable[assembler::trim(token)] + " "; 
                    } else {
                        processedLine += token + " ";
                    }
                }
        }
        }
        else if (uncommentedLine.find("start:") == 0 || uncommentedLine.find("START:") == 0
                || uncommentedLine.find("inicio:") == 0 || uncommentedLine.find("INICIO:") == 0) { //case where the program starts
            startFound = true; //we are at line 0 of the program

            while (iss >> token) {
                if (token.front() == '[' && token.back() == ']') {
                    // Extract word inside brackets
                    std::string wordInsideBrackets = token.substr(1, token.length() - 2);
                    
                    if (symbolTable.find(wordInsideBrackets) != symbolTable.end()) {
                        processedLine += "[" + symbolTable[wordInsideBrackets] + "] "; // Add brackets back and change the word inside
                    } else {
                        processedLine += token + " ";
                    }
                } else {
                    if (symbolTable.find(assembler::trim(token)) != symbolTable.end()) {
                        processedLine += symbolTable[assembler::trim(token)] + " "; 
                    } else {
                        processedLine += token + " ";
                    }
                }
            }
        
        }

        if (!processedLine.empty()) {
            // Remove the trailing space and write the processed line
            out_file << processedLine.substr(0, processedLine.size() - 1) << std::endl;
        }

    }

    in_file.close();
    out_file.close();

    std::cout << "second pass complete" << std::endl;
    //third pass: find all line address labels, store their address number (the line number) and name to the map

    assembler::toUpperFile("processed_program.txt"); // Convert the processed program to uppercase

    std::ifstream out_file2("processed_program.txt"); // Open the outfile again to restart the file

    if (!out_file2.is_open()) {
        std::cerr << "Failed to open file on third pass!" << std::endl;
        return;	
    }

    while (std::getline(out_file2, line)) {
        std::istringstream iss(line);
        std::string label;
        std::string instruction;
        std::string operand;

        if (iss >> label >> instruction >> operand) {
            if (label.back() == ':')
            {
                lineAddressTable[assembler::trim(label.substr(0, label.size() - 1))] = assembler::trim(std::to_string(lineNumber));
                //removes the ":" from the label and stores the label and the line number (as a string) in the map
            }
        }

        lineNumber++; //increment line number
    }

    out_file2.close();

    std::cout << "third pass complete" << std::endl;
    //fourth pass: find all jump labels and exchange them for their address number

    std::ifstream out_file3("processed_program.txt"); // Open the outfile again to restart the file
    std::ofstream temp("temp.txt"); // use temp to hold the processed program

    while (std::getline(out_file3, line)) {
        std::string unlabeledLine = line.substr(line.find(':') + 1); // Remove labels
        std::istringstream iss(unlabeledLine);
        std::string instruction;
        std::string operand;
        std::string processedLine;

        while (iss >> instruction >> operand) {
            if (lineAddressTable.find(assembler::trim(operand)) != lineAddressTable.end())
            {
                operand = lineAddressTable[assembler::trim(operand)];
            }

            processedLine += instruction + " " + operand;
        }

        if (!processedLine.empty()) {
            temp << processedLine << std::endl;
        }
    }

    out_file3.close();
    temp.close();

    std::ofstream out_file4("processed_program.txt"); // writing back from temp
    std::ifstream temp2("temp.txt");

    while (std::getline(temp2, line)) {
        out_file4 << line << std::endl; // Write temp file line to the output file
    }

    temp2.close();
    out_file4.close();

    std::ofstream temp3("temp.txt", std::ofstream::out | std::ofstream::trunc); //erase contents of temp file
    temp3.close();

    std::cout << "Fourth pass complete - Preprocessing complete" << std::endl;
}

void assembler::finalAssembly(std::ifstream &file, std::ofstream &out_file) {

    std::string line;

    while (std::getline(file, line)) {
        std::string processedLine;
        std::string unlabeledLine = line.substr(line.find(':') + 1); // Remove labels
        std::istringstream iss(unlabeledLine);
        std::string instruction;
        std::string operand;

        while (iss >> instruction >> operand) {

            std::string memoryAddress;
            std::string opcode;

            if (assembler::trim(operand).front() == '[') // If operand is a memory address
            {
                memoryAddress = assembler::trim(operand).substr(1, assembler::trim(operand).length() - 2); // Remove brackets
                
                if (memoryAddress.back() == 'H') { // address is in hexadecimal
                    if (memoryAddress.length() > 3) { // if the address is bigger than 16 bits
                        std::cerr << "Invalid memory address: " << memoryAddress << " Cant be bigger than FF" << std::endl;
                        return;
                    }
                    
                    memoryAddress = assembler::hexToBinary(memoryAddress);
                }
                else {
                    memoryAddress = assembler::decimalToBinary(memoryAddress);
                    if (memoryAddress.length() > 8) { // if the address is bigger than 8 bits
                        std::cerr << "Invalid memory address: " << memoryAddress << " Max 8 bits" << std::endl;
                        return;
                    }
                }

                if (instructionMap.find(instruction) != instructionMap.end()) {
                    opcode = instructionMap[instruction].memory;
                } else {
                    std::cerr << "Invalid instruction: " << instruction << std::endl;
                    return;
                }

            } else {    //if operand is a value for immediate access
                if (operand.back() == 'H') { // address is in hexadecimal
                    if (operand.length() > 3) { // if the address is bigger than 16 bits
                        std::cerr << "Invalid operand: " << operand << "Cant be bigger than FF" << std::endl;
                        return;
                    }
                    
                    memoryAddress = assembler::hexToBinary(operand);
                }
                else {
                    memoryAddress = assembler::decimalToBinary(operand);
                    if (memoryAddress.length() > 8) { // if the address is bigger than 8 bits
                        std::cerr << "Invalid operand: " << memoryAddress << "Max 8 bits" << std::endl;
                        return;
                    }
                }

                if (instructionMap.find(instruction) != instructionMap.end()) {
                    opcode = instructionMap[instruction].immediate;
                } else {
                    std::cerr << "Invalid instruction: " << instruction << std::endl;
                    return;
                }
            }
            
            processedLine += opcode + memoryAddress;

            if (!processedLine.empty()) {
                out_file << processedLine << std::endl;
            }

            std::cout << "processed line: " << processedLine << std::endl; //debugging
        }
    }

    std::cout << "Program was assembled sucessfully!" << std::endl;
}
