#include <iostream>
#include <sys/mman.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/stat.h>
#include "../lib/error_constants.h"

int main(int argc, char* argv[]) {
    std::string file;
    std::cout << "Введите имя файла: ";
    std::cin >> file;
    file = "../" + file;
    const char *file_name = file.c_str();
    int input = open(file_name, O_RDONLY);
    if (input == -1) {
        std::cerr << "No such file\n";
        return FILE_ERROR;
    }
    const char* name = "../array_mem";
    int fd = open(name, O_CREAT|O_RDWR);
    if (fd == -1) {
        std::cerr << "No such file\n";
        return FILE_ERROR;
    }

    struct stat st;
    if (fstat(input, &st) == -1) {
        std::cerr << "fstat problem\n";
        return FSTAT_ERROR;
    }
    const int64_t SIZE_OF_FILE = st.st_size;

    if (ftruncate(fd, SIZE_OF_FILE) == -1) {
        std::cerr << "ftrunc problem\n";
        return FTRUNC_ERROR;
    }

    void * pmmap = mmap(0, SIZE_OF_FILE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pmmap == MAP_FAILED) {
        std::cerr << "memory map is not allocated\n";
        return MMAP_ERROR;
    }

    int *arr = (int*)(pmmap);
    arr[0] = 0;

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "fork problem\n";
        return FORK_ERROR;
    } else if (pid == 0) {
        if (dup2(input, fileno(stdin)) == -1) {
            std::cerr << "dup2 problem\n";
            return DUP2_ERROR;
        }
        execl("./child", "child", name, nullptr);
    } else {
        int index = 1;
        while(true) {
            if (arr[0] == -1) {
                if (munmap(pmmap, SIZE_OF_FILE) == -1) {
                    std::cerr << "munmap problem\n";
                    return MUNMAP_ERROR;
                }
                if (close(fd) == -1) {
                    std::cerr << "close error\n";
                    return CLOSE_ERROR;
                }
                return 0;
            }
            if (arr[0]) {
                for (int i = 0;  i < arr[0]; ++i) {
                    std::cout << arr[index++] << std::endl;
                }
                arr[0] = 0;
            }
        }
    }

    return 0;

}
