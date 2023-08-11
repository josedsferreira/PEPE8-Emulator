#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

class compiler
{

public:
    compiler();
    ~compiler();

    //structures and variables:
    // Token types
    enum TokenType {
        INSTRUCTION,
        CONSTANT,
        ADDRESS,
        IDENTIFIER,
        UNKNOWN
    };

    // Token structure
    struct Token {
        TokenType type;
        std::string value;
    };

    //Functions:
    // Function to tokenize a line
    std::vector<Token> tokenizeLine(const std::string& line);

};