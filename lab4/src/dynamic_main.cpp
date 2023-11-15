#include <iostream>
#include <dlfcn.h>


void text(bool flag) {
    if (flag) {
        std::cout << std::endl;
        std::cout << "Введите 1, если хотите перевести число в двоичную систему счисления\n";
        std::cout << "Введите 2, если хотите перевести число в троичную систему счисления\n";
        std::cout << "Введите 0, если хотите поменять способ перевода числа, по умолчанию число переводится в двоичную систему счисления\n";
    }
}


typedef char * (*Pointer_to_function)(long);


using namespace std;


int main() {
    int operation = 1;
    void * hdl = dlopen("libNumSystems.so", RTLD_LAZY);
    if (hdl == nullptr) {
        throw std::logic_error("library not loaded");
    }

    Pointer_to_function from_decimal_to_binary = reinterpret_cast<Pointer_to_function>(dlsym(hdl, "from_decimal_to_binary"));
    if (from_decimal_to_binary == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    Pointer_to_function from_decimal_to_ternary = reinterpret_cast<Pointer_to_function>(dlsym(hdl, "from_decimal_to_ternary"));
    if (from_decimal_to_ternary == nullptr) {
        throw std::logic_error("lib not loaded");
    }

    int t;
    bool flag = false;

    cout << "Введите 1, если хотите перевести число в двоичную систему счисления\n";
    cout << "Введите 2, если хотите перевести число в троичную систему счисления\n";
    cout << "Введите 0, если хотите поменять способ перевода числа, по умолчанию число переводится в двоичную систему счисления\n";

    while (cin >> t) {
        flag = true;
        int x;
        auto help = [&]() -> string {
            string res = "в двоичную";
            if (t == 2 || (t == 0 && operation == 1))
                res = "в троичную";
            return res;
        };
        string help_info = help();

        if (t == 0) {
            cout << "Введите число, которое хотите перевести(" << help_info << "): ";
            cin >> x;

            operation ^= 1;
            if (operation == 1) {
                cout << "Число в двоичном представлении: " << from_decimal_to_binary(x) << endl;
                text(flag);
            } else {
                cout << "Число в троичном представлении: " << from_decimal_to_ternary(x) << endl;
                text(flag);
            }
        } else if (t == 1) {
            cout << "Введите число, которое хотите перевести(" << help_info << "): ";
            cin >> x;

            cout << "Число в двоичном представлении: " << from_decimal_to_binary(x) << endl;
            text(flag);
        } else if (t == 2) {
            cout << "Введите число, которое хотите перевести(" << help_info << "): ";
            cin >> x;

            cout << "Число в троичном представлении: " << from_decimal_to_ternary(x) << endl;
            text(flag);
        } else {
            if (dlclose(hdl) == -1)
                throw std::logic_error("Ошибка закрытия динамической библиотеки");

            throw std::invalid_argument("Некоректные входные данные");
        }
    }

    if (dlclose(hdl) == -1)
        throw std::logic_error("Ошибка закрытия динамической библиотеки");

    return 0;
}
