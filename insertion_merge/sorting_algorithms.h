#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <stdint.h>

// Main sorting functions
void insertion_merge(uint64_t* L, uint64_t len);
void dual_pivot_quicksort(uint64_t* L, uint64_t len);

#endif