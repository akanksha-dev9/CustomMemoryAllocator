#include "../include/MemoryPool.h"
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
    pool=nullptr;
    head=nullptr;
}

void* MemoryPool::allocate(size_t size){
    BlockHeader* current=head;
    
    while(current){
        if(current->is_free && current->size>=size){
            if(current->size>size+sizeof(BlockHeader*)+1){
                BlockHeader* new_block=(BlockHeader*)((char*)current+sizeof(BlockHeader)+size);
                new_block->size=current->size-size-sizeof(BlockHeader*);
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
    std::cout<<"Memory allocation failed. Not enough space."<<std::endl;
    return nullptr;
}

void MemoryPool::deallocate(void* ptr){
    if(ptr){
        BlockHeader* block=(BlockHeader*)((char*)ptr-sizeof(BlockHeader));
        block->is_free=true;
        coalesce();
    }
}

void MemoryPool::coalesce(){
    BlockHeader* current=head;

    while(current && current->next){
        if(current->is_free && current->next->is_free){
            current->size+=sizeof(BlockHeader)+current->next->size;
            current->next=current->next->next;
        }
        current=current->next;
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
