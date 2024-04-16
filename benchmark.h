#pragma once
#include <chrono>
#include <vector>
#include <string>

#define SECONDS 1
#define MILLISECONDS 2
#define MICROSECONDS 3
#define NANOSECONDS 4

class Timer {
public:

    Timer() = delete;
    
    Timer(int ratio,std::string functionName,size_t iterations);

    void Start();
    void Stop();

    std::string units;
    std::vector<unsigned long long int> timePerCall{};
    std::string textFileName;


private:
    std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
    unsigned long long int timeMeasure;
    unsigned long long int maxTime = 0;

};

void writeBenchmark(const Timer& benchmark);



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
auto runStandardBenchmark(const Ratio ratio,const FunctionName functionName,Iterations iterations,const Function f,Args...args){
    Timer benchmark(ratio,functionName,iterations);

    while(iterations-- > 0){
        benchmark.Start();
        f(args...);
        benchmark.Stop();
    }

    writeBenchmark(benchmark);
}



