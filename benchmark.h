#pragma once
#include <chrono>
#include <vector>
#include <fstream>
#include <numeric>
#include <memory>

namespace Benchmark{
    constexpr const unsigned short int SECONDS = 1;
    constexpr const unsigned short int MILLISECONDS = 2;
    constexpr const unsigned short int MICROSECONDS = 3;
    constexpr const unsigned short int NANOSECONDS = 4;

    #define U128 unsigned long long


    class Timer {
    public:

        Timer() = delete;
        
        Timer(const int ratio,const std::string functionName,const long long iterations);

        ~Timer(){writeBenchmark();};

        void Start();
        void Stop();

        std::string units = " ns";
        std::vector<unsigned long long int> timePerCall{};
        std::string textFileName;


    private:

        void writeBenchmark();

        std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
        unsigned short int timeMeasure = 4;
        U128 maxTime = 0;
        float multiplier = 1;

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

        std::unique_ptr<Timer> benchmark = std::make_unique<Timer>(ratio,functionName,iterations);

        f(args...); //Cold run

        while(iterations-- > 0){
            benchmark->Start();
            f(args...);
            benchmark->Stop();
        }

    }

        
} // namespace Benchmark


