#include "common.hpp"

void line_eof(void) {
    if(std::cin.eof()) {
        exit(1);
    }
}