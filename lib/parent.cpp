#include "parent.h"
#include "child.h"

int parent_process(int fd[2]) {
    close(fd[1]);
    int64_t answer;
    if (read(fd[0], &answer, sizeof(int64_t)) == -1) {
        std::cout << "Somthing wrong with read" << std::endl;
        return READ_PROBLEM;
    }
    close(fd[0]);
    std::cout << answer << std::endl;
    return GOOD;
}
