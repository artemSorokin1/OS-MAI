#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    try {
        const char *name = argv[1];
        int fd = open(name, O_CREAT | O_RDWR);
        int *arr = (int *) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


        int sum = 0;
        int x;
        int index = 1;
        bool end = false;
        int cnt = 0;
        while (true) {

            if (end && arr[0] == 0) {
                arr[0] = -1;
                if (munmap(arr, 4096) == -1) {
                    throw std::string("munmap error");
                }

                if (close(fd) == -1) {
                    throw std::string("close error");
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
    catch (std::exception & ex) {
        std::cout << ex.what() << '\n';
    }

}