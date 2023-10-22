#include <iostream>
#include <sys/mman.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>



int main(int argc, char* argv[]) {

    try {
        std::string file;
        std::cout << "Введите имя файла: ";
        std::cin >> file;
        file = "../" + file;
        const char *file_name = file.c_str();
        int input = open(file_name, O_RDONLY);
        if (input == -1)
            throw std::string("No such file");

        const char* name = "../array_mem";
        int fd = open(name, O_CREAT|O_RDWR);
        if (fd == -1) {
            throw std::string("No such file");
        }


        ftruncate(fd, 4096);

        void * pmmap = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (pmmap == MAP_FAILED)
            throw std::string("memory map is not allocated");

        int *arr = (int*)(pmmap);

        arr[0] = 0;

        pid_t pid = fork();

        if (pid == -1) {
            throw std::string("fork error");
        } else if (pid == 0) {
            if (dup2(input, fileno(stdin)) == -1)
                throw std::string("dup2 error");
            execl("./child", "child", name, nullptr);
        } else {
            int index = 1;
            while(true) {
                if (arr[0] == -1) {
                    if (munmap(pmmap, 4096) == -1)
                        throw std::string("munmap error");

                    if (close(fd) == -1)
                        throw std::string("close error");
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
