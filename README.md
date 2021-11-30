# Demand-Paging-Virtual-Memory-Simulator
Implements FIFO, OPT, LRU, and LFU algorithms in C++ to operate demand paging in Virtual Memory. Accepts a reference string
via user input and performs demand paging using various algorithms, providing victim frames and faults. 

## diff with raw repo

- change `opt`, `lru`, `lfu` some logic struction to justify my task.
  - add test function for above algo.
- general test data: `7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`.