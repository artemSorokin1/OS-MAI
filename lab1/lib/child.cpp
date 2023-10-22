#include <iostream>
#include "constants.h"


int main() {
    int sum = 0;
    int x;
    try {
        while (std::cin.peek() != EOF) {
            if (std::cin.peek() == '\n') {
                std::cin.ignore();
                std::cout << sum << '\n';
                sum = 0;
            } else {
                std::cin >> x;
                sum += x;
            }
        }

    } catch (...) {
        std::cout << "ERROR" << '\n';
    }
    return GOOD;
}

