// sorting.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include "sorting_algorithms.h"

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

void measure_time(char* fname, void (*fptr)(uint64_t*, uint64_t), uint64_t* jumbled_arr, uint64_t* sorted_arr, uint64_t len) {

    // Copy the jumbled array
    uint64_t* arr = malloc(sizeof(uint64_t) * len);
    memcpy(arr, jumbled_arr, sizeof(uint64_t) * len);

    // Declare timing measurement variables
    clock_t start, end;
    double duration;

    // Apply algorithm and and measure time
    start = clock();
    fptr(arr, len);
    end = clock();
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Validate results
    int sorted_correctly = 1;
    for (uint64_t i = 0; i < len; i++) {
        if (arr[i] != sorted_arr[i]) {
            sorted_correctly = 0;
            break;
        }
    }

    // Print results
    printf("%25s time: %f seconds\n", fname, duration);
    if (!sorted_correctly) {
        printf("WARNING: %s does not agree with qsort.\n", fname);
    }

}

void test_algos(uint64_t* jumbled_arr, uint64_t len) {

    uint64_t* sorted_arr;
    {
        // Copy the jumbled array
        uint64_t* arr = malloc(sizeof(uint64_t) * len);
        memcpy(arr, jumbled_arr, sizeof(uint64_t) * len);

        // Declare timing measurement variables
        clock_t start, end;
        double duration;

        // Apply qsort and measure time
        start = clock();
        qsort(arr, len, sizeof(uint64_t), compare_uint64_t);
        end = clock();
        duration = ((double) (end - start)) / CLOCKS_PER_SEC;

        // Print qsort time
        printf("%25s time: %f seconds\n", "qsort", duration);

        // Use the result of qsort as our sorted array
        sorted_arr = arr;

    }

    // Try some other sorts
    measure_time("dual_pivot_quicksort", dual_pivot_quicksort, jumbled_arr, sorted_arr, len);
    measure_time("insertion_merge", insertion_merge, jumbled_arr, sorted_arr, len);

    // Free memory and exit
    free(sorted_arr);

}

int main() {

    /* First Test */ {
        printf("Fully jumbled list\n");
        uint64_t len = 10000000;
        uint64_t* jumbled_arr = generate_random_list(len);
        test_algos(jumbled_arr, len);
        free(jumbled_arr);
    }

    /* First Test */ {
        printf("Partially jumbled list\n");
        uint64_t len = 10000000;
        uint64_t* jumbled_arr = generate_somewhat_sorted_list(len, 3);
        test_algos(jumbled_arr, len);
        free(jumbled_arr);
    }

    // Exit
    return 0;

}