#include "benchmark.h"
#include <assert.h>

#define ASSERT(message,condition) assert(((void)message,condition))

Timer::Timer(const int ratio,const std::string functionName,const size_t iterations)
    : timeMeasure(ratio),textFileName(functionName){

    ASSERT("Iterations must have a value higher than 0",iterations > 0);
    
    switch (ratio) {
        case SECONDS: units[1] = 's',multiplier = 0.000000001f; break;
        case MILLISECONDS: units[1] = 'm',units[2] = 's',multiplier = 0.000001f; break;
        case MICROSECONDS: units[1] = 'u',units[2] = 's',multiplier = 0.000001f; break;
        case NANOSECONDS: units[1] = 'n',units[2] = 's',multiplier = 0.001f; break;
        default:break;

    }
    timePerCall.reserve(iterations+1);

}

Timer::~Timer(){

    if (timePerCall.size() == 0) return;

    unsigned long long int sumOfCallTimes = std::accumulate(timePerCall.begin(),timePerCall.end(),0);

    std::ofstream saveData(std::format("{}.md",textFileName),std::fstream::out);
    if (saveData.is_open()) {

        saveData << "#Function :" << textFileName << "\n\n";
        saveData << "## Average call time: " << sumOfCallTimes / timePerCall.size() << units << "\n\n";
        saveData << "## Calls to function: " << timePerCall.size() << "\n\n";

        for(const unsigned long long& callTime : timePerCall)
            saveData << callTime << units << "\n\n";

        saveData.close();
    }
}

void Timer::Start(){
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

#ifdef DEBUGPRINT
#include <thread>
#include <iostream>
void Timer::Stop() {
    const auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

    unsigned long long duration = (end - start)*multiplier;

    timePerCall.push_back(duration);
 
   //The first time this class is constructed the latency is really high so we ignore the first result


#define DEBUG(x) do { std::cout << x << '\n'; } while (0)

    if (duration > maxTime && timePerCall.size() != 0) maxTime = duration;
   
    DEBUG(duration << units);
    DEBUG("Max time spent in 1 call: " << maxTime << units);
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); //Sleep before clearing so you can see it
    system("cls");

}
#else
void Timer::Stop() {
    const auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

    unsigned long long duration = (end - start)*multiplier;

    timePerCall.push_back(duration);
 
   //The first time this class is constructed the latency is really high so we ignore the first result

}
#endif
