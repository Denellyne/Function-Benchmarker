#include "benchmark.h"
#include <fstream>
#include <format>

std::vector<Timer> timers; 

Timer* runStandardBenchmark(int ratio, std::string functionName){
    Timer benchmark(ratio,functionName);
    timers.push_back(benchmark);
    return &timers[timers.size()-1];
}

inline void writeBenchmark(const Timer& benchmark);

void saveOnExit(){
    for(unsigned long long int  i = 0;i < timers.size();i++) 
        writeBenchmark(timers[i]);
}

inline void writeBenchmark(const Timer& benchmark) {
    //The reason we do it here instead of when we are pushing the data to the vector
    //is so we minimize the bloat inside our Timer Class
    if (benchmark.timePerCall.size() == 0) return;

    unsigned long long int sumOfCallTimes = 0;

    for (unsigned long long int i = 1; i < benchmark.timePerCall.size() - 1; i++)
        sumOfCallTimes += benchmark.timePerCall[i];

    std::ofstream saveData(std::format("{}.md",benchmark.textFileName),std::fstream::out);
    if (saveData.is_open()) {

        saveData << std::format("#Function: {}\n\n",benchmark.textFileName);
        saveData << "## Average call time: " << sumOfCallTimes / benchmark.timePerCall.size() << benchmark.units << '\n' << '\n';
        saveData << "## Calls to function: " << benchmark.timePerCall.size() << '\n' << '\n';

        for (unsigned long long int i = 0; i < benchmark.timePerCall.size(); i++)
            saveData << benchmark.timePerCall[i] << benchmark.units << '\n' << '\n';

        saveData.close();
    }
}

Timer::Timer(int ratio,std::string functionName){
    timeMeasure = ratio;
    textFileName = functionName;

    switch (ratio) {
    case 1: units = " s"; break;
    case 2: units = " ms"; break;
    case 3: units = " us"; break;
    case 4: units = " ns"; break;

    default:
        timeMeasure = 4;
        units = " ns";
        break;
    }

}

void Timer::Start(){
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}
void Timer::Stop() {
    auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

    int duration = end - start;

    switch (timeMeasure) {
    case 1:
        duration *= 0.000000001;
        break;
    case 2:
        duration *= 0.000001;
        break;
    case 3:
        duration *= 0.001;
        break;
    }


 
   //The first time this class is constructed the latency is really high so we ignore the first result
#ifdef DEBUGPRINT
    if (duration > maxTime && timePerCall.size() != 0) maxTime = duration;
   
    DEBUG(duration << units);
    DEBUG("Max time spent in 1 call: " << maxTime << units);
    Sleep(1); //Sleep before clearing so you can see it
    system("cls");
#endif
    timePerCall.push_back(duration);

}