#include <iostream>
#include <unistd.h>
#include <fstream>
#include "constants.h"
#include <string>


int main() {
    int64_t answer;
    if (read(fileno(stdin), &answer, sizeof(int64_t)) != 8) {
        std::cout << "READ_PROBLEM" << '\n'; // с большой вероятностью ошибка здесь 
        return READ_PROBLEM;
    }
    std::cout << answer << std::endl;
    return GOOD;
}


