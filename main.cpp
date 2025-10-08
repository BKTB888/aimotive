#include <fstream>
#include <iostream>
#include "Field.h"


int main() {

    const auto input = "---------------------------------\n"
                       "--T----3-X---------X-----A-------\n"
                       "-------------------------X-------\n"
                       "---X---X------X-------------3----\n"
                       "------------------X------4---X---\n"
                       "---1--X-------C-X--------X-------\n"
                       "---X-----------------------------";
    std::cout << input << "\n\n";
    auto field = Field(input);

    std::cout << field << "\n\n";

    auto fs = std::ifstream("input.txt");

    fs >> field;
    std::cout << field << "\n\n";

    return 0;
}
