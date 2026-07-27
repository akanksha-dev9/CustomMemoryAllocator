# 🧠 Custom Memory Allocator (C++17)

A high-performance, robust memory pool allocator built from scratch in C++17.  
Achieves **4.81x faster** allocation than standard `malloc` — benchmarked over 1 million iterations.

---

## ✨ Features

- ✅ **Memory Pool** — Single OS allocation, internally self-managed
- ✅ **Block Splitting** — Large free blocks split on demand to minimize waste
- ✅ **Coalescing** — Adjacent free blocks merged automatically to prevent fragmentation
- ✅ **Move Semantics** — Rule of Five implemented; copy disabled, move supported
- ✅ **Double-Free Detection** — Warns instead of silently corrupting state
- ✅ **Invalid Pointer Guard** — Rejects pointers outside pool bounds
- ✅ **Exception Safety** — Throws `std::invalid_argument` for undersized pool; catches `std::bad_alloc`
- ✅ **Leak Detector** — Macro-based tracking with exact file name and line number reporting
- ✅ **Benchmarks** — Honest comparison using `volatile` sink to prevent dead code elimination

---

## 📁 Project Structure

```
MemoryAllocator/
├── include/
│   ├── MemoryPool.h          ← Core allocator + BlockHeader
│   └── LeakDetector.h        ← Leak tracking macros
├── src/
│   ├── MemoryPool.cpp        ← allocate, deallocate, coalesce, move semantics
│   └── LeakDetector.cpp      ← Allocation registry + leak reporting
├── tests/
│   └── testAllocator.cpp     ← 7 regression tests
├── benchmark/
│   └── bench.cpp             ← malloc vs custom pool speed comparison
├── .gitignore
├── Makefile
└── README.md
```

---

## 🚀 How to Run

```bash
# Clone
git clone https://github.com/akanksha-dev9/CustomMemoryAllocator.git
cd CustomMemoryAllocator

# Run tests
make run

# Run benchmark
make run-bench
```

---

## 📊 Benchmark Results

> Compiler: g++ -O2 | Iterations: 1,000,000 | Alloc size: 64 bytes  
> `volatile` sink used to prevent compiler dead code elimination

| Method       | Time          |
|--------------|---------------|
| `malloc/free`| 74,127 μs     |
| Custom Pool  | 15,383 μs     |
| **Speedup**  | **4.81x faster** ✅ |

---

## 🔍 How It Works

### Allocation — First Fit Strategy
```
Pool (initialized once from OS):
[ Header | FREE ................................ ]

After allocate(64):
[ Header | 64B USED ][ Header | FREE .......... ]

After allocate(128):
[ Header | 64B USED ][ Header | 128B USED ][ Header | FREE ]
```

### Deallocation + Coalescing
```
After deallocate(ptr1):
[ Header | 64B FREE ][ Header | 128B USED ][ Header | FREE ]

Coalesce — adjacent free blocks merged:
[ Header | FREE (merged) ][ Header | 128B USED ][ Header | FREE ]
```

### Leak Detection
```
allocate(100) → registry: { ptr, 100 bytes, file.cpp, line 42 }
deallocate()  → registry: { }  ← removed
program end   → registry not empty = LEAK ⚠️
```

---

## 🧪 Test Coverage

| Test | What It Validates |
|------|------------------|
| `testBasicAllocation` | Allocate, deallocate, reuse |
| `testThreeWayCoalesce` | 3+ adjacent free blocks fully merge |
| `testSplitBoundaryNoCorruption` | Split arithmetic doesn't corrupt next block's header |
| `testDoubleFreeIsCaught` | Second free prints warning, doesn't crash |
| `testInvalidPointerIsCaught` | Out-of-pool pointer rejected safely |
| `testUnderflowGuard` | Pool smaller than `BlockHeader` throws exception |
| `testMoveSemantics` | Move constructor transfers ownership; no double-free |

---

## 🏗️ Design Decisions

### Embedded Free List
Free list is intentionally embedded inside `MemoryPool` via the `BlockHeader` linked list — no separate class needed.

**Why?**
- Better cache locality — header and data are contiguous in memory
- No extra pointer overhead
- Single traversal tracks both free and used blocks

### Rule of Five
`MemoryPool` owns a raw heap resource, so:
- Copy constructor and copy assignment are **deleted** — prevents double-free
- Move constructor and move assignment are **implemented** — safe ownership transfer
- Destructor explicitly releases the pool back to OS

---

## 💡 Concepts Covered

- Pointer arithmetic and manual memory management
- Linked list based block tracking
- Memory fragmentation and coalescing
- RAII — Resource Acquisition Is Initialization
- Rule of Five — move semantics and resource ownership
- Exception safety — `bad_alloc`, `invalid_argument`
- Honest performance benchmarking with `std::chrono` and `volatile`
- Macro-based instrumentation (`__FILE__`, `__LINE__`)

---

## 🔮 Upcoming Improvements

- [ ] Templated allocator — `pool.allocate<int>()` typed interface
- [ ] STL-compatible allocator for use with `std::vector`, `std::list`
- [ ] Thread-safe allocator with `std::mutex`
- [ ] Lock-free allocator with atomics
- [ ] Memory alignment support (`alignas`)
- [ ] Best-fit allocation strategy

---

## 👩‍💻 Author

**Akanksha** — MCA Student | Systems Programming Enthusiast  
Built as a systems programming portfolio project to demonstrate low-level memory management in C++17.

[![GitHub](https://img.shields.io/badge/GitHub-akanksha--dev9-black?logo=github)](https://github.com/akanksha-dev9)