#include "program_1.h"


int GCD(int a, int b) {
    while (a % b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return b;
}


int GCD_week(int a, int b) {
    int result = 1;
    for (int i = 1; i <= std::min(a, b); ++i) {
        if (a % i == 0 && b % i == 0) {
            result = i;
        }
    }
    return result;
}
