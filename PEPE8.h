#include <array>
#include "dataUnit.h"
#include "ctrlUnit.h"

class PEPE8 {

public:
    PEPE8();
    ~PEPE8();

    //2 Parts of the processor:
    dataUnit DU;
    ctrlUnit CU;

    //2 memories (teoretically they exist outside the processor):
    std::array<uint8_t, 1 * 1024> data; //1KB not sure how much i need
    std::array<uint8_t, 15> instructions; //15 8bit instructions
};