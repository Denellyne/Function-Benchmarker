#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <format>
#include <numeric>

constexpr const unsigned short int SECONDS = 1;
constexpr const unsigned short int MILLISECONDS = 2;
constexpr const unsigned short int MICROSECONDS = 3;
constexpr const unsigned short int NANOSECONDS = 4;

class Timer {
public:

    Timer() = delete;
    
    Timer(const int ratio,const std::string functionName,const size_t iterations);
    ~Timer();

    void Start();
    void Stop();

    char units[4] = " ns";
    std::vector<unsigned long long int> timePerCall{};
    std::string textFileName;


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    unsigned short int timeMeasure = 4;
    unsigned long long int maxTime = 0;
    float multiplier = 0.001f;

};

/// @brief Makes x iterations of the function and writes all call times to a .md file
/// @param ratio 
///    1 -> Seconds,
///    2 -> Milliseconds,
///    3 -> Microseconds,
///    4 -> Nanoseconds,
/// @param functionName  Function Name -> Name for the resulting textfile
/// @param Iterations Number of iterations
/// @param Function The function you wish to benchmark
template <typename Ratio,typename FunctionName,typename Iterations,typename Function, class ... Args>
void runStandardBenchmark(const Ratio ratio,const FunctionName functionName,Iterations iterations,const Function f,Args...args){
    Timer benchmark(ratio,functionName,iterations);
    unsigned iter = iterations;
    while(iterations-- > 0){
        benchmark.Start();
        f(args...);
        benchmark.Stop();
    }
}



