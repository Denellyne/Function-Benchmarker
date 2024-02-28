#pragma once
#include <windows.h>
#include <chrono>
#include <vector>
#include <iostream>
//#define DEBUGPRINT

#define DEBUG(x) do { std::cout << x << '\n'; } while (0)

void saveOnExit();
/*
If you wish to write all your data to a text file then call this function in the beggining of the main function

If you wish to Print the call times In-Real-Time then uncomment the #define DEBUGPRINT on benchmark.h
Beware this might make call latency higher

If you already have a Close App Callback function then you can call saveOnExit there.
*/
#define setupBenchmark atexit(saveOnExit);


/*
    If you get a very high maxTime right after starting the program then do a Sleep(5000)
    in your main function right before the function you want to benchmark
    so your other threads have time to finish their work
*/


class Timer {
public:

    Timer() = delete;
    
    Timer(int ratio,std::string functionName);

    void Start();
    void Stop();

    ~Timer() {
        Stop();
    }

    std::string units;
    std::vector<unsigned long long int> timePerCall{};
    std::string textFileName;


private:
    std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
    unsigned long long int timeMeasure;
    unsigned long long int maxTime = 0;

};




/// @brief Do Timer->Start() at the start of the Function and Timer->Stop() at the end of the function to benchmark
/// @param ratio 
///    1 -> Seconds,
///    2 -> Milliseconds,
///    3 -> Microseconds,
///    4 -> Nanoseconds,
/// @param functionName  Function Name -> Name for the resulting textfile
/// @return Returns a pointer to a Timer Object
Timer* runStandardBenchmark(int ratio,std::string functionName);
