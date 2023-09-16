#include "../lib/child.h"
#include "../lib/parent.h"


int main() {
    std::string file_name;
    std::cout << "Введите имя файла: ";
    std::cin >> file_name;
    std::ifstream file;

    int fd[2];
    if (pipe(fd) == -1) {
        std::cout << "Somthing wrong with pipe" << std::endl;
        return PIPE_PROBLEM;
    }
    pid_t pid = fork();

    if (pid == -1) {
        std::cout << "Something wrong with fork" << std::endl;
        return FORK_PROBLEM;
    } else if (pid == 0) {
        child_process(fd, file_name, file);
    } else {
        parent_process(fd);
    }

    return GOOD;
}




