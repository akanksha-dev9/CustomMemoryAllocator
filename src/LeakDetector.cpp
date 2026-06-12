#include "../include/LeakDetector.h"
#include <iostream>
#include <cstddef>
#include <vector>

LeakDetector globalLeakDetector;

void LeakDetector::trackAlloc(void* address, size_t size, const char* file, int line){
    leaks.push_back({address, size, file, line});
}

void LeakDetector::trackDealloc(void* address){
    for(auto it=leaks.begin(); it!=leaks.end(); it++){
        if(it->address==address){
            leaks.erase(it);
            return;
        }
    }
    std::cout<<"Warning: Attempt to deallocate untracked memory at "<<address<<std::endl;
}

void LeakDetector::reportLeaks() const{
    if(leaks.empty()){
        std::cout<<"No memory leaks detected."<<std::endl;
    }
    else{
        std::cout<<"Memory leaks detected: "<<std::endl;
        for(const auto &leak: leaks){
            std::cout<<"Leaked "<<leak.size<<" bytes at "<<leak.address<<" allocated in "<<leak.file<<": "<<leak.line<<std::endl;
        }
    }
}