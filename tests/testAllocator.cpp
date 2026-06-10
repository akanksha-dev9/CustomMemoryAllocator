#include "../include/MemoryPool.h"
#include <cstring>
#include <cassert>
#include <iostream>

void testMemoryPool(){
    MemoryPool pool(1024);

    std::cout<<"Testing MemoryPool..."<<std::endl;

    pool.printMemoryPool();

    void* ptr1=pool.allocate(256);
    assert(ptr1!=nullptr);
    std::cout<<"Allocated 256 bytes."<<std::endl;

    pool.printMemoryPool();

    void* ptr2=pool.allocate(128);
    assert(ptr2!=nullptr);
    std::cout<<"Allocated 128 bytes."<<std::endl;

    pool.printMemoryPool();

    pool.deallocate(ptr1);
    std::cout<<"Deallocated 256 bytes."<<std::endl;

    pool.printMemoryPool();

    void* ptr3=pool.allocate(64);
    assert(ptr3!=nullptr);  
    std::cout<<"Allocated 64 bytes."<<std::endl;

    pool.printMemoryPool();
    std::cout<<"Used Memory: "<<pool.usedMemory()<<std::endl;

}

int main(){
    testMemoryPool();
    return 0;
}