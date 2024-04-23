#include "benchmark.h"
#include <assert.h>
#include <format>

#define ASSERT(message,condition) assert((message,condition))

using namespace Benchmark;

Timer::Timer(const int ratio,const std::string functionName,const long long iterations)
    : timeMeasure(ratio),textFileName(functionName){

    ASSERT("Iterations must have a value higher than 0",iterations > 0);
    
    switch (ratio) {
        case SECONDS: units = "s";multiplier = 0.000000001f; break;
        case MILLISECONDS: units = "ms"; multiplier = 0.000001f; break;
        case MICROSECONDS: units = "us"; multiplier = 0.000001f; break;
        case NANOSECONDS:  units = "ns"; multiplier = 0.001f; break;

        default: break;

    }
    timePerCall.reserve(iterations+1);

}

void Timer::Start(){
    m_StartTimepoint = 
        std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
    const auto endTimePoint = 
        std::chrono::high_resolution_clock::now();

    auto start = 
        std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();

    auto end =  
        std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

    U128 duration = (end - start)*multiplier;

    timePerCall.push_back(duration);
 
#ifdef DEBUGPRINT
#include <iostream>

#define DEBUG(x) do { std::cout << x << '\n'; } while (0)

    if (duration > maxTime) maxTime = duration;
   
    DEBUG(duration << units);
    DEBUG("Max time spent in 1 call: " << maxTime << units);
    Sleep(1); //Sleep before clearing so you can see it
    system("cls");
#endif

}

void Timer::writeBenchmark() {
        //The reason we do it here instead of when we are pushing the data to the vector
        //is so we minimize the bloat inside our Timer Class
        if (timePerCall.size() == 0) return;

        U128 sumOfCallTimes = 
            std::accumulate(timePerCall.begin(),timePerCall.end(),0);

        std::ofstream saveData(std::format("{}.md",textFileName),std::fstream::out);
        
        if (saveData.is_open()) {

            saveData << "# Function :" << textFileName << "\n\n";
            saveData << "## Average call time: " << sumOfCallTimes / timePerCall.size() << units << "\n\n";
            saveData << "## Calls to function: " << timePerCall.size() << "\n\n";

            for(const unsigned long long& callTime : timePerCall)
                saveData << "### " << callTime << units << "\n\n";

            saveData.close();
        }

        else fprintf(stderr,"Failed to save data\n");
}