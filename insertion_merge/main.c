// sorting.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include "insertion_merge.h"

// Comparator function for qsort
int compare_uint64_t(const void* a, const void* b) {
    uint64_t ua = *((const uint64_t*)a);
    uint64_t ub = *((const uint64_t*)b);
    if (ua < ub) return -1;
    if (ua > ub) return 1;
    return 0;
}

// Function to generate a random list of uint64_t values in the heap
uint64_t* generate_random_list(uint64_t length) {
    // Check if the desired length is 0
    if (length == 0) {
        return NULL; // Return NULL for an empty list
    }

    // Allocate memory for the list on the heap
    uint64_t* list = (uint64_t*)malloc(sizeof(uint64_t) * length);

    // Check if memory allocation was successful
    if (list == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate random values and populate the list
    for (uint64_t i = 0; i < length; i++) {
        list[i] = (uint64_t)rand(); // You can use rand() or any other random function
    }

    return list; // Return the pointer to the generated list
}

uint64_t* generate_somewhat_sorted_list(uint64_t length, uint64_t max_swap_gap) {
    if (length == 0) {
        return NULL;
    }

    // Generate a random list first
    uint64_t* list = generate_random_list(length);

    // Sort the list in ascending order
    qsort(list, length, sizeof(uint64_t), compare_uint64_t);

    // Introduce small swaps to make the list somewhat sorted
    srand(time(NULL));
    for (uint64_t i = 0; i < length; i++) {
        uint64_t swap_gap = rand() % (max_swap_gap + 1);
        if (i + swap_gap < length) {
            // Swap the items at indices i and i + num_swaps
            uint64_t temp = list[i];
            list[i] = list[i + swap_gap];
            list[i + swap_gap] = temp;
        }
    }

    return list;
}

int main() {
    uint64_t len = 10000000;  // 1 million items
    // uint64_t* arr1 = generate_random_list(len);
    uint64_t* arr1 = generate_somewhat_sorted_list(len, 2);
    uint64_t* arr2 = malloc(sizeof(uint64_t) * len);

    // Copy the random list to arr2
    memcpy(arr2, arr1, sizeof(uint64_t) * len);

    // Timing measurement variables
    clock_t start, end;
    double insertion_merge_time, qsort_time;

    // Apply insertion_merge and measure time
    start = clock();
    insertion_merge(arr1, len);
    end = clock();
    insertion_merge_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Apply qsort and measure time
    start = clock();
    qsort(arr2, len, sizeof(uint64_t), compare_uint64_t);
    end = clock();
    qsort_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Validate results
    int sorted_correctly = 1;
    for (uint64_t i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            sorted_correctly = 0;
            break;
        }
    }

    if (sorted_correctly) {
        printf("Both algorithms produced the same sorted output.\n");
    } else {
        printf("The sorted outputs differ.\n");
    }

    printf("Insertion Merge Time: %f seconds\n", insertion_merge_time);
    printf("qsort Time: %f seconds\n", qsort_time);

    // Clean up
    free(arr1);
    free(arr2);

    return 0;
}