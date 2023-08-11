#include "compiler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

compiler::compiler()
{
}

compiler::~compiler()
{
}

std::vector<compiler::Token> compiler::tokenizeLine(const std::string& line) {
        std::vector<Token> tokens;
        std::istringstream iss(line); // Create a stream from the line
        std::string tokenStr;

        while (iss >> tokenStr) {
            Token token;
            tokenStr = tokenStr.substr(0, tokenStr.find(';')); // Remove comments
            
            for (char &c : tokenStr) {
                c = std::toupper(c);
            }
            
            if (tokenStr == "LD" || tokenStr == "ST" || tokenStr == "ADD" || tokenStr == "SUB"
            || tokenStr == "AND" || tokenStr == "OR" || tokenStr == "JMP" || tokenStr == "JZ"
            || tokenStr == "JN" || tokenStr == "NOP") {
                token.type = INSTRUCTION;
            } else if (isdigit(tokenStr[0])) {
                token.type = CONSTANT;
            } else if (tokenStr[0] == '[') {
                token.type = ADDRESS;
            } else {
                token.type = IDENTIFIER;
            }

            token.value = tokenStr;
            tokens.push_back(token);
        }

        return tokens;
    }