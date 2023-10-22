#include <iostream>
#include <sys/mman.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstdlib>


int sizeOfFileLines(std::string name) {
    int cnt = 0;
    std::fstream file(name, std::ios::in);
    std::string temp;
    bool end = false;
    while (std::getline(file, temp)) {
        if (temp == "" && end) {
            break;
        } else if (temp == "" && !end) {
            end = true;
        } else {
            end = false;
        }
        cnt++;
    }
    return cnt;
}



int main(int argc, char* argv[]) {

    try {
        std::string file;
        std::cout << "Введите имя файла: ";
        std::cin >> file;
        file = "../" + file;
        const char *file_name = file.c_str();
        int input = open(file_name, O_RDONLY);
        if (input == -1) {
            throw std::string("No such file");
        }

        int fd = open("../array_mem", O_RDWR);
        if (fd == -1) {
            throw std::string("No such file");
        }


        int fileSize = sizeof(int) * sizeOfFileLines(file_name);

        ftruncate(fd, fileSize);

        int *pmmap = static_cast<int *>(mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        if (pmmap == MAP_FAILED)
            throw std::string("mmap failed");


        pid_t pid = fork();

        if (pid == -1) {
            throw std::string("fork failed");
        } else if (pid == 0) {
            if (dup2(input, fileno(stdin)) == -1)
                throw std::string("dup2 failed");
            int sum = 0;
            int x;
            int index = 0;
            while (std::cin.peek() != EOF) {
                if (std::cin.peek() == '\n') {
                    std::cin.ignore();
                    pmmap[index++] = sum;
                    sum = 0;
                } else {
                    std::cin >> x;
                    sum += x;
                }
            }

//        close(input);
        } else {
            waitpid(pid, NULL, 0);
            for (int i = 1; i <= fileSize / sizeof(int); ++i) {
                std::cout << pmmap[i] << '\n';
            }
        }

        if (munmap(pmmap, fileSize) == -1)
            throw std::string("munmap failed");

        if (close(fd) == -1)
            throw std::string("close failed");

        return 0;
    }
    catch (std::string & s) {
        std::cout << "Error: " << s << '\n';
    }
    catch (std::exception & ex) {
        std::cout <<  "Error: " << ex.what() << '\n';
    }
    catch (...) {
        std::cout << "Some error" << '\n';
    }
}
