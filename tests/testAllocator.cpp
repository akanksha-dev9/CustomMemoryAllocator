#include "../include/MemoryPool.h"
#include "../include/LeakDetector.h"
#include <cstring>
#include <cassert>
#include <iostream>

void testBasicAllocation(){
    std::cout<<"=== testBasicAllocation ==="<<std::endl;
    MemoryPool pool(1024);
    pool.printMemoryPool();

    void* ptr1=pool.allocate(256);
    assert(ptr1!=nullptr);
    TRACK_ALLOC(ptr1, 256);
    std::cout<<"Allocated 256 bytes."<<std::endl;
    pool.printMemoryPool();

    void* ptr2=pool.allocate(128);
    assert(ptr2!=nullptr);
    TRACK_ALLOC(ptr2, 128);
    std::cout<<"Allocated 128 bytes."<<std::endl;
    pool.printMemoryPool();

    pool.deallocate(ptr1);
    TRACK_DEALLOC(ptr1);
    std::cout<<"Deallocated 256 bytes."<<std::endl;
    pool.printMemoryPool();

    void* ptr3=pool.allocate(64);
    assert(ptr3!=nullptr);
    TRACK_ALLOC(ptr3, 64);
    std::cout<<"Allocated 64 bytes."<<std::endl;
    pool.printMemoryPool();

    std::cout<<"Used Memory: "<<pool.usedMemory()<<std::endl;
    REPORT_LEAKS();
    std::cout<<std::endl;
}

void testThreeWayCoalesce(){
    std::cout<<"=== testThreeWayCoalesce ==="<<std::endl;
    MemoryPool pool(512);

    void* a=pool.allocate(64);
    void* b=pool.allocate(64);
    void* c=pool.allocate(64);
    (void)a; (void)b; (void)c;

    std::cout<<"Before freeing A, B, C:"<<std::endl;
    pool.printMemoryPool();

    pool.deallocate(a);
    pool.deallocate(b);
    pool.deallocate(c);

    std::cout<<"After freeing all three (should coalesce into ONE big free block"<<std::endl;
    std::cout<<"at the front, not stay split):"<<std::endl;
    pool.printMemoryPool();
    std::cout<<std::endl;
}

void testSplitBoundaryNoCorruption(){
    std::cout<<"=== testSplitBoundaryNoCorruption ==="<<std::endl;
    MemoryPool pool(256);

    void* ptr1=pool.allocate(64);
    void* ptr2=pool.allocate(64);
    assert(ptr1 && ptr2);

    std::memset(ptr1, 0xAB, 64);

    std::cout<<"Used memory after two 64-byte allocations: "<<pool.usedMemory()<<std::endl;
    pool.printMemoryPool();

    pool.deallocate(ptr1);
    pool.deallocate(ptr2);
    std::cout<<"Used memory after freeing both: "<<pool.usedMemory()<<std::endl;
    std::cout<<std::endl;
}

void testDoubleFreeIsCaught(){
    std::cout<<"=== testDoubleFreeIsCaught ==="<<std::endl;
    MemoryPool pool(256);
    void* ptr=pool.allocate(32);
    pool.deallocate(ptr);
    std::cout<<"Second deallocate() below should print a double-free warning, not crash:"<<std::endl;
    pool.deallocate(ptr); 
    std::cout<<std::endl;
}

void testInvalidPointerIsCaught(){
    std::cout<<"=== testInvalidPointerIsCaught ==="<<std::endl;
    MemoryPool pool(256);
    int stackVar=42;
    std::cout<<"Deallocating a stack address below should print a warning, not crash:"<<std::endl;
    pool.deallocate(&stackVar); // not from this pool at all
    std::cout<<std::endl;
}

void testUnderflowGuard(){
    std::cout<<"=== testUnderflowGuard ==="<<std::endl;
    try{
        MemoryPool tiny(4); // smaller than sizeof(BlockHeader)
        std::cout<<"ERROR: expected an exception for an undersized pool!"<<std::endl;
    }
    catch(const std::invalid_argument& e){
        std::cout<<"Correctly rejected undersized pool: "<<e.what()<<std::endl;
    }
    std::cout<<std::endl;
}

void testMoveSemantics(){
    std::cout<<"=== testMoveSemantics ==="<<std::endl;
    MemoryPool pool1(256);
    void* ptr=pool1.allocate(32);
    (void)ptr;

    MemoryPool pool2(std::move(pool1)); // move ctor: pool2 now owns the buffer
    std::cout<<"After move, pool2 used memory: "<<pool2.usedMemory()<<std::endl;
    // pool1 is now empty/moved-from; its destructor must not double-free.
    std::cout<<std::endl;
}

int main(){
    testBasicAllocation();
    testThreeWayCoalesce();
    testSplitBoundaryNoCorruption();
    testDoubleFreeIsCaught();
    testInvalidPointerIsCaught();
    testUnderflowGuard();
    testMoveSemantics();

    std::cout<<"All tests completed."<<std::endl;
    return 0;
}