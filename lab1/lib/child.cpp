#include <iostream>
#include <unistd.h>
#include <fstream>
#include "constants.h"
#include <cstdlib>
#include <cstdio>


int main() {
    int64_t sum = 0;
    char c;
    std::string s;
    while(std::getline(std::cin, s)) {
        sum = 0;
        std::string res = "";
        for (char & elem : s) {
            if (elem == ' ') {
                sum += stoll(res);
                res = "";
            } else res += elem;
        }
        if (write(fileno(stdout), &sum, sizeof(int64_t)) != 8)
            return WRITE_PROBLEM;
    }
    
    close(fileno(stdin));

    return GOOD;
}
