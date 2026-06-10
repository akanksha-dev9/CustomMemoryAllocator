#include "MemoryPool.h"
#include <cstring>

MemoryPool::MemoryPool(size_t size){
    pool=::operator new(size);
    pool_size=size;
    
    if(pool){
        head=(BlockHeader*)pool;
        head->size=size-sizeof(BlockHeader);
        head->is_free=true;
        head->next=nullptr;
    }
    else{
        std::cout<<"Failed to allocate memory pool."<<std::endl;
        head=nullptr;
    }
}

MemoryPool::~MemoryPool(){
    ::operator delete(pool);
}