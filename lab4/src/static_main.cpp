#include "../lib/program_1.h"
#include <iostream>

using namespace std;


int main() {
    int operation;
    int a, b;
    cout << "Введите 1, если хотите найти НОД при помощи алгоритма Евклида\n";
    cout << "Введите 2, если хотите найти НОД при помощи наивного алгоритма\n";
    cout << "После ввода номера операции, введеите два аргумента 'a' и 'b' : числа, НОД которых хотите найти\n";
    while (cin >> operation >> a >> b) {
        if (cin.fail()) {
            throw std::invalid_argument("Некоректные входные данные");
        }
        if (operation == 1) {
            int gcd = GCD(a, b);
            printf("НОД(a, b) = %d\n", gcd);
        } else if (operation == 2) {
            int gcd = GCD_week(a, b);
            printf("НОД(a, b) = %d\n", gcd);
        } else {
            throw std::invalid_argument("Некоректные входные данные");
        }
    }

    return 0;
}
