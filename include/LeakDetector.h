#pragma once
#include <cstddef>
#include <vector>

class LeakDetector{
private:
    struct LeakInfo{
        void* address;
        size_t size;
        const char* file;
        int line;
    };
public:
    std::vector<LeakInfo> leaks;

    void trackAlloc(void* address, size_t size, const char* file, int line);
    void trackDealloc(void* address);
    void reportLeaks() const;
};

extern LeakDetector globalLeakDetector;

#define TRACK_ALLOC(ptr,size) globalLeakDetector.trackAlloc(ptr, size, __FILE__, __LINE__)
#define TRACK_DEALLOC(ptr) globalLeakDetector.trackDealloc(ptr)
#define REPORT_LEAKS() globalLeakDetector.reportLeaks()