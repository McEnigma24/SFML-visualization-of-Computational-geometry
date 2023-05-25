#pragma once
#include "Header.h"


ScopeTimer::ScopeTimer(string n, string f)
{
    start = std::chrono::system_clock::now();
    name = n;
    file = f;
    new_line = false;
}
ScopeTimer::ScopeTimer(string n, string f, int nl)
{
    start = std::chrono::system_clock::now();
    name = n;
    file = f;
    new_line = true;
}

ScopeTimer::~ScopeTimer()
{
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> d = stop - start;

    cout << name << " took: " << (d.count() / 1000) << "s" << endl;


    #if BENCHMARKING_FILE_SAVE == 1

        std::ofstream FILE;
        // std::ios::app is the open mode "append" meaning
        // new data will be written to the end of the file.
        FILE.open(file.c_str(), std::ios::app);

        if (FILE.good())    
        FILE << name << " took: " << (d.count() / 1000) << "s" << endl;
        if (new_line) FILE << endl;
    
    #endif
}