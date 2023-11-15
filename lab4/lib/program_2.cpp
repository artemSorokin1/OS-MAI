#include "program_2.h"


size_t len_of_result_string(long x, int base) {
    size_t len = 0;
    while (x > 0) {
        ++len;
        x /= base;
    }
    return len;
}


char * from_decimal_to_binary(long x){
    const size_t len = len_of_result_string(x, 2);
    char * result = new char[len];
    int i = len - 1;
    while (x > 0) {
        new (result + i) char(x % 2 + '0');
        x /= 2;
        --i;
    }
    return result;
}


char * from_decimal_to_ternary(long x){
    const size_t len = len_of_result_string(x, 3);
    char * result = new char[len];
    int i = len - 1;
    while (x > 0) {
        new (result + i) char(x % 3 + '0');
        x /= 3;
        --i;
    }
    return result;
}


void cleaner(const char * s) {
    delete[] s;
}



