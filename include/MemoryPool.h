#pragma once
#include <iostream>
#include <cstddef>
#include <vector>

struct BlockHeader{
    size_t size;
    bool is_free;
    BlockHeader* next;
};

class MemoryPool {
private:
    void* pool;
    size_t pool_size;
    BlockHeader* head;
    void coalesce();
public:
    MemoryPool(size_t size);
    ~MemoryPool();
    void* allocate(size_t size);
    void deallocate(void* ptr);
    void printMemoryPool() const;
    size_t usedMemory() const;
};