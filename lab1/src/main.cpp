#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include "../lib/constants.h"


int main() {
    std::string file;
    std::cout << "Введите имя файла: ";
    std::cin >> file;
    file = "../" + file;
    const char* file_name = file.c_str();
    int input = open(file_name, O_RDONLY );
    if (input == -1) {
        std::cout << "такого файла нет" << std::endl;
        return FILE_PROBLEM;
    }

    int fd[2];
    if (pipe(fd) == -1) {
        return PIPE_PROBLEM;
    }

    pid_t pid = fork();

    if (pid == -1) {
        return FORK_PROBLEM;
    } else if (pid == 0) {
        close(fd[0]);
        const char* path = "./child";
        if (dup2(fd[1], fileno(stdout)) == -1) {
            return DUP_ERROR;
        }
        if (dup2(input, fileno(stdin)) == -1) {
            return DUP_ERROR;
        }
        if (execl(path, path, NULL) == -1)
            return EXEC_ERROR;
    } else {
        close(fd[1]);
        if (dup2(fd[0], fileno(stdin)) == -1) {
            std::cout << "dup2_err" << '\n'; 
            return DUP_ERROR;
        }
        const char* path = "./parent";
        if (execl(path, path, NULL) == -1)
            return EXEC_ERROR;
    }
    

    return GOOD;
}




