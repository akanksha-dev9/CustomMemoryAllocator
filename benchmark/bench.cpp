#include <iostream>
#include <chrono>
#include <cstdlib>
#include "../include/MemoryPool.h"

int main(){
    const size_t sizeAlloc= 64;
    const int iterations= 1000000;

    //malloc allocation and deallocation time
    auto start1= std::chrono::high_resolution_clock::now();
    for(int i=0;i<iterations;i++){
        void* ptr= std::malloc(sizeAlloc);
        std::free(ptr);
    }
    auto end1= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> malloc_time=end1-start1;

    //memory pool allocation and deallocation time
    MemoryPool pool((sizeAlloc+24)*iterations);
    
    auto start2= std::chrono::high_resolution_clock::now();
    for(int i=0;i<iterations;i++){
        void *ptr=pool.allocate(sizeAlloc);
        pool.deallocate(ptr);
    }
    auto end2= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> pool_time=end2-start2;

    //results
    std::cout << "Iterations     : " << iterations << "\n";
    std::cout << "Alloc size     : " << sizeAlloc << " bytes each\n\n";
    std::cout << "malloc/free    : " << malloc_time.count() << " milliseconds\n";
    std::cout << "Custom Pool    : " << pool_time.count() << " milliseconds\n\n";

    if (pool_time < malloc_time) {
        double speedup = malloc_time.count() / pool_time.count();
        std::cout << "Custom Pool is " << speedup << "x faster than malloc!\n";
    } else {
        std::cout << "Malloc was faster this run (pool size too large, cache miss)\n";
        std::cout << "Tip: Use smaller pool size for better cache performance.\n";
    }

    return 0;
}