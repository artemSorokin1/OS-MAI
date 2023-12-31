#include <iostream>
#include <dlfcn.h>

void text() {
    std::cout << std::endl;
    std::cout << "Введите 1, если хотите вызвать функцию поиска НОДа двух чисел\n";
    std::cout << "Введите 2, если хотите перевести число в двоичную или троичную систему счисления\n";
    std::cout << "Введите 0, если хотите поменять функцию(по умолчанию поиск НОДа)\n";
}

typedef char * (*Pointer_to_transaction_function)(long);
typedef void (*Pointer_to_cleaner_function)(const char *);
typedef int (*Pointer_to_gcd_function)(int, int);
using namespace std;

int main() {
    bool hdl1_close = false;
    bool hdl2_close = false;
    int operation = 1;
    void * hdl1 = dlopen("libNumSystems.so", RTLD_LAZY);
    if (hdl1 == nullptr) {
        throw std::logic_error("library not loaded");
    }

    void * hdl2 = dlopen("libGCD.so", RTLD_LAZY);
    if (hdl2 == nullptr) {
        throw std::logic_error("library not loaded");
    }

    Pointer_to_transaction_function binary = reinterpret_cast<Pointer_to_transaction_function>
    (dlsym(hdl1, "from_decimal_to_binary"));
    if (binary == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    Pointer_to_transaction_function ternary = reinterpret_cast<Pointer_to_transaction_function>
    (dlsym(hdl1, "from_decimal_to_ternary"));
    if (ternary == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    Pointer_to_gcd_function GCD = reinterpret_cast<Pointer_to_gcd_function>(dlsym(hdl2, "GCD"));
    if (GCD == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    Pointer_to_gcd_function GCD_week = reinterpret_cast<Pointer_to_gcd_function>(dlsym(hdl2, "GCD_week"));
    if (GCD_week == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    Pointer_to_cleaner_function cleaner = reinterpret_cast<Pointer_to_cleaner_function>(dlsym(hdl1, "cleaner"));
    if (cleaner == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    int t;
    cout << "Введите 1, если хотите вызвать функцию поиска НОДа двух чисел\n";
    cout << "Введите 2, если хотите перевести число в двоичную или троичную систему счисления\n";
    cout << "Введите 0, если хотите поменять функцию(по умолчанию поиск НОДа)\n";

    auto GCD_prog = [&]() {
        if (!hdl1_close) {
            if (dlclose(hdl1) == -1)
                throw std::logic_error("Ошибка закрытия динамической библиотеки");
            hdl1_close = true;
        }
        if (hdl2_close) {
            hdl2 = dlopen("libGCD.so", RTLD_LAZY);
            if (hdl2 == nullptr) {
                throw std::logic_error("library not loaded");
            }
            hdl2_close = false;
        }
        int number;
        cout << "Введите 1, если хотите использовать функцию GCD\n";
        cout << "Введите 2, если хотите использовать функцию GCD_week\n";
        cin >> number;
        int x, y;
        if (number == 1) {
            Pointer_to_gcd_function GCD = reinterpret_cast<Pointer_to_gcd_function>(dlsym(hdl2, "GCD"));
            if (GCD == nullptr) {
                throw std::logic_error("lib not loaded");
            }
            cout << "Введите числа, НОД которых хотите найти: ";
            cin >> x >> y;
            cout << "НОД(" << x << ", " << y << ") = " << GCD(x, y) << endl;
            text();
        } else if (number == 2) {
            Pointer_to_gcd_function GCD_week = reinterpret_cast<Pointer_to_gcd_function>(dlsym(hdl2, "GCD_week"));
            if (GCD_week == nullptr) {
                throw std::logic_error("lib not loaded");
            }
            cout << "Введите числа, НОД которых хотите найти: ";
            cin >> x >> y;
            cout << "НОД(" << x << ", " << y << ") = " << GCD_week(x, y) << endl;
            text();
        }
    };

    auto translate_prog = [&]() {
        if (!hdl2_close) {
            if (dlclose(hdl2) == -1)
                throw std::logic_error("Ошибка закрытия динамической библиотеки");
            hdl2_close = true;
        }
        if (hdl1_close) {
            hdl1 = dlopen("libNumSystems.so", RTLD_LAZY);
            if (hdl1 == nullptr) {
                throw std::logic_error("library not loaded");
            }
            hdl1_close = false;
        }
        int number;
        cout << "Введите 1, если хотите использовать функцию перевода числа в двоичную СС\n";
        cout << "Введите 2, если хотите использовать функцию перевода числа в троичную СС\n";
        cin >> number;
        if (number == 1) {
            Pointer_to_transaction_function binary = reinterpret_cast<Pointer_to_transaction_function>
            (dlsym(hdl1, "from_decimal_to_binary"));
            if (binary == nullptr) {
                throw std::logic_error("lib not loaded");
            }
            long x;
            cout << "Введите число, которое хотите перевести: ";
            cin >> x;
            char * res = binary(x);
            cout << "Результат: " << res << endl;
            cleaner(res);
            text();
        } else if (number == 2) {
            Pointer_to_transaction_function ternary = reinterpret_cast<Pointer_to_transaction_function>
            (dlsym(hdl1, "from_decimal_to_ternary"));
            if (ternary == nullptr) {
                throw std::logic_error("lib not loaded");
            }
            long x;
            cout << "Введите число, которое хотите перевести: ";
            cin >> x;
            char * res = ternary(x);
            cout << "Результат: " << res << endl;
            cleaner(res);
            text();
        }
    };

    while (cin >> t) {
        if (t == 0) {
            cout << "Функция изменена\n";
            operation ^= 1;
            if (operation == 1) {
                GCD_prog();
            } else {
                translate_prog();
            }
        }
        else if (t == 1) {
            GCD_prog();
        }
        else if (t == 2) {
            translate_prog();
        }
        else {
            if (!hdl1_close) {
                if (dlclose(hdl1) == -1)
                    throw std::logic_error("Ошибка закрытия динамической библиотеки");
            }
            if (!hdl2_close) {
                if (dlclose(hdl2) == -1)
                    throw std::logic_error("Ошибка закрытия динамической библиотеки");
            }
        }
    }
    if (!hdl1_close) {
        if (dlclose(hdl1) == -1)
            throw std::logic_error("Ошибка закрытия динамической библиотеки");
    }
    if (!hdl2_close) {
        if (dlclose(hdl2) == -1)
            throw std::logic_error("Ошибка закрытия динамической библиотеки");
    }
    return 0;
}


