#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include "../lib/constants.h"



// через относительный путь почему-то не работает (../../build/child   ../../build/parent) 

int main() {
    std::string file;
    std::cout << "Введите имя файла: ";
    std::cin >> file;
    file = "../lab1/" + file;
    const char* file_name = file.c_str();
    int out = open(file_name, O_WRONLY );
    if (out == -1) {
        std::cout << "такого фала нет" << std::endl;
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
        const char* path = "/Users/artemsorokin/CLionProjects/OS/build/child";
        if (dup2(fd[1], fileno(stdout)) == -1) {
            return DUP_ERROR;
        }
        if (dup2(out, fileno(stdin)) == -1) {
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
        const char* path = "/Users/artemsorokin/CLionProjects/OS/build/parent";
        if (execl(path, path, NULL) == -1)
            return EXEC_ERROR;
    }
    

    return GOOD;
}




