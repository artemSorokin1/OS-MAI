#include <iostream>
#include <chrono>

class LocalTimer {
private:
    std::chrono::time_point<std::chrono::steady_clock> _start, _end;
    std::chrono::duration<double> _duration;
    bool isStart;
    bool isStop;
public:
    LocalTimer();
    double timeNow();
    void timeStop();
    void timeStart();
};