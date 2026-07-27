#pragma once
#include <iostream>
#include <cstddef>
#include <vector>
#include <new>
#include <stdexcept>

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
    bool isValidBlock(BlockHeader* block) const;
public:
    explicit MemoryPool(size_t size);

    ~MemoryPool();
    MemoryPool(const MemoryPool&) = delete; //copy constructor deleted
    MemoryPool& operator=(const MemoryPool&) = delete;  //copy assignment operator deleted
    MemoryPool(MemoryPool&& other) noexcept;  //move constructor
    MemoryPool& operator=(MemoryPool&& other) noexcept; //move assignment operator

    void* allocate(size_t size);
    void deallocate(void* ptr);
    void printMemoryPool() const;
    size_t usedMemory() const;
};