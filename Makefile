CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Directories
INC      = include
SRC      = src
TEST     = tests
BENCH    = benchmark

# Source files (common)
SRCS     = $(SRC)/MemoryPool.cpp $(SRC)/LeakDetector.cpp

# default: test build
all: test

# test build
test: $(SRCS) $(TEST)/testAllocator.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) $(SRCS) $(TEST)/testAllocator.cpp -o test_allocator.exe
	@echo Build successful! Run: test_allocator.exe

# benchmark build
bench: $(SRCS) $(BENCH)/bench.cpp
	$(CXX) $(CXXFLAGS) -O2 -I$(INC) $(SRCS) $(BENCH)/bench.cpp -o bench.exe
	@echo Benchmark ready! Run: bench.exe

# run test
run: test
	./test_allocator.exe

# run benchmark
run-bench: bench
	./bench.exe

# clean — Windows
clean:
	del /f test_allocator.exe bench.exe 2>nul || true
	@echo Cleaned!