#include "TimerManager.h"

LocalTimer::LocalTimer() : _start(std::chrono::high_resolution_clock::now()), _end(std::chrono::high_resolution_clock::now()), isStart(false), isStop(false) {}

double LocalTimer::timeNow() {  // stop && !start || !stop && start || !stop && !start
    if (isStart) {
        _end = std::chrono::high_resolution_clock::now();
        _duration = _end - _start;
    } else if (!isStart && isStop || !isStart && !isStop) {
        _duration = _end - _start;
    }
    return _duration.count();
}

void LocalTimer::timeStart() {
    _start = std::chrono::high_resolution_clock::now();
    isStart = true;
    isStop = false;
}

void LocalTimer::timeStop() {
    _end = std::chrono::high_resolution_clock::now();
    isStop = true;
    isStart = false;
}


