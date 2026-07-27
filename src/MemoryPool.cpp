#include "../include/MemoryPool.h"
#include <cstring>

MemoryPool::MemoryPool(size_t size){
    if(size <= sizeof(BlockHeader)){
        throw std::invalid_argument("MemoryPool size must be greater than sizeof(BlockHeader)");
    }

    pool = nullptr;
    try{
        pool = ::operator new(size); //allocate raw memory for the pool
    }
    catch(const std::bad_alloc&){
        std::cout<<"Failed to allocate memory pool."<<std::endl;
        pool=nullptr;
        pool_size=0;
        head=nullptr;
        throw;  //Construction failed
    }

    pool_size=size;
    head=(BlockHeader*)pool;
    head->size=size-sizeof(BlockHeader);
    head->is_free=true;
    head->next=nullptr;
}

MemoryPool::~MemoryPool(){
    ::operator delete(pool);  //delete the allocated memory pool
    pool=nullptr;
    head=nullptr;
    pool_size=0;
}

MemoryPool::MemoryPool(MemoryPool&& other) noexcept {
    pool=other.pool;
    pool_size=other.pool_size;
    head=other.head;
    
    other.pool=nullptr;
    other.pool_size=0;
    other.head=nullptr;
}

MemoryPool& MemoryPool::operator=(MemoryPool&& other) noexcept{   
    if(this!=&other){
        ::operator delete(pool); // release whatever this pool currently owns
        pool=other.pool;
        pool_size=other.pool_size;
        head=other.head;

        other.pool=nullptr;
        other.pool_size=0;
        other.head=nullptr;
    }
    return *this;
}

void* MemoryPool::allocate(size_t size){
    if(size==0) return nullptr;

    BlockHeader* current=head;

    while(current){
        if(current->is_free && current->size>=size){
            if(current->size>=size+sizeof(BlockHeader)+1){   //Block splitting: only split if the remaining space can hold a new BlockHeader and at least 1 byte of data
                BlockHeader* new_block=(BlockHeader*)((char*)current+sizeof(BlockHeader)+size);
                new_block->size=current->size-size-sizeof(BlockHeader);
                new_block->is_free=true;
                new_block->next=current->next;

                current->size=size;
                current->next=new_block;
            }
            current->is_free=false;
            return (char*)current+sizeof(BlockHeader);
        }
        current=current->next;
    }
    std::cout<<"Memory allocation failed: not enough space for "<<size<<" bytes."<<std::endl;
    return nullptr;
}

bool MemoryPool::isValidBlock(BlockHeader* block) const{
    // Walk the real block list rather than trusting the pointer arithmetic
    // blindly. This catches garbage pointers, pointers from a different
    // pool, and pointers into the middle of a block.
    for(BlockHeader* current=head; current!=nullptr; current=current->next){
        if(current==block) return true;
    }
    return false;
}

void MemoryPool::deallocate(void* ptr){
    if(!ptr) return;

    if(ptr<pool || ptr>=(char*)pool+pool_size){      //  ptr....[pool....pool+pool_size]....ptr (ptr is outside the pool range)
        std::cout<<"Warning: deallocate() called with a pointer outside this pool: "<<ptr<<std::endl;
        return;
    }

    BlockHeader* block=(BlockHeader*)((char*)ptr-sizeof(BlockHeader));

    if(!isValidBlock(block)){
        std::cout<<"Warning: deallocate() called with an invalid/corrupted pointer: "<<ptr<<std::endl;
        return;
    }

    if(block->is_free){
        std::cout<<"Warning: double free detected at "<<ptr<<std::endl;
        return;
    }

    block->is_free=true;
    coalesce();
}

void MemoryPool::coalesce(){
    BlockHeader* current=head;

    while(current && current->next){
        if(current->is_free && current->next->is_free){
            current->size+=sizeof(BlockHeader)+current->next->size;
            current->next=current->next->next;
        }
        else{
            current=current->next;
        }
    }
}

void MemoryPool::printMemoryPool() const{
    BlockHeader* current=head;
    size_t offset=0;

    while(current){
        std::cout<<"Block at offset "<<offset<<" : size = "<<current->size<<" bytes, "<<(current->is_free?" free ":" allocated ")<<std::endl;
        offset+=sizeof(BlockHeader)+current->size;
        current=current->next;
    }
}

size_t MemoryPool::usedMemory() const{
    size_t used=0;
    BlockHeader* current=head;

    while(current){
        if(!current->is_free){
            used+=current->size;
        }
        current=current->next;
    }
    return used;
}