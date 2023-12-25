#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("Incorect job id");
    }
    int job_id = stoi(argv[1]);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Job with id = " << job_id << " has worked" << endl;

    return 0;
}