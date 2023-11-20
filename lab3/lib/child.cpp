#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error_constants.h"


int main(int argc, char* argv[]) {
    const char *name = argv[1];
    int fd = open(name, O_CREAT | O_RDWR);
    if (fd == -1) {
        std::cerr << "open error\n";
        return FILE_ERROR;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        std::cerr << "fstat problem\n";
        return FSTAT_ERROR;
    }
    const int64_t SIZE_OF_FILE = st.st_size;

    int *arr = (int *) mmap(0, SIZE_OF_FILE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (arr == MAP_FAILED) {
        std::cerr << "mmap problem\n";
        return MMAP_ERROR;
    }
    int sum = 0;
    int x;
    int index = 1;
    bool end = false;
    int cnt = 0;
    while (true) {
        if (end && arr[0] == 0) {
            arr[0] = -1;
            if (munmap(arr, SIZE_OF_FILE) == -1) {
                std::cerr << "munmap problem\n";
                return MUNMAP_ERROR;
            }
            if (close(fd) == -1) {
                std::cerr << "close problem\n";
                return CLOSE_ERROR;
            }
            return 0;
        }
        if (arr[0] == 0) {
            cnt = 0;
            while (cnt < 5) {
                if (std::cin.peek() == EOF) {
                    end = true;
                    break;
                }
                if (std::cin.peek() == '\n') {
                    std::cin.ignore();
                    arr[index++] = sum;
                    sum = 0;
                    cnt++;
                } else {
                    std::cin >> x;
                    sum += x;
                }
            }
            arr[0] = cnt;
        }
    }
}