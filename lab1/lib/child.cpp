#include "child.h"


int child_process(int fd[2], std::string &file_name, std::ifstream &file) {
    close(fd[0]);
    file.open(file_name, std::ifstream::in);
    if (!file.is_open()) {
        std::cout << "Такого файла нет" << std::endl;
        return NO_SUCH_FILE;
    } else {
        int x;
        int64_t sum = 0;
        while (file >> x) {
            sum += x;
        }
        if (write(fd[1], &sum, sizeof(int64_t)) == -1) {
            std::cout << "Somthing wrong with write" << std::endl;
            return WRITE_PROBLEM;
        }
    }
    file.close();
    close(fd[1]);

    return GOOD;
}



