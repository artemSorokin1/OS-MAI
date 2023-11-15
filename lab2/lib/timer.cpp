#include <iostream>
#include <chrono>


class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<double> duration;
public:

    Timer();
    ~Timer();
    auto get_duration() { return duration.count(); };
};

Timer::Timer() {
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout.precision(7);
    std::cout << std::fixed << "time: " << duration.count() << std::endl;
}

