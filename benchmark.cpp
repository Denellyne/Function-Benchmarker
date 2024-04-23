#include "benchmark.h"

Timer::Timer(const int ratio,const std::string functionName,const size_t iterations)
: timeMeasure(ratio),textFileName(functionName){

    switch (ratio) {
        case SECONDS: units[1] = 's',multiplier = 0.000000001f; break;
        case MILLISECONDS: units[1] = 'm',units[2] = 's',multiplier = 0.000001f; break;
        case MICROSECONDS: units[1] = 'u',units[2] = 's',multiplier = 0.000001f; break;
        case NANOSECONDS: units[1] = 'n',units[2] = 's',multiplier = 0.001f; break;

        default:
            units[1] = 'n',units[2] = 's';
            break;
        }
    timePerCall.reserve(iterations+1);

}

void Timer::Start(){
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}
void Timer::Stop() {
    const auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

    unsigned long long duration = (end - start)*multiplier;

    timePerCall.push_back(duration);

 
   //The first time this class is constructed the latency is really high so we ignore the first result
#ifdef DEBUGPRINT
#include <iostream>

#define DEBUG(x) do { std::cout << x << '\n'; } while (0)

    if (duration > maxTime && timePerCall.size() != 0) maxTime = duration;
   
    DEBUG(duration << units);
    DEBUG("Max time spent in 1 call: " << maxTime << units);
    Sleep(1); //Sleep before clearing so you can see it
    system("cls");
#endif

}