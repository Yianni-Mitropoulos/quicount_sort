#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

void swap(int64_t *a, int64_t *b) {
    int64_t temp = *a;
    *a = *b;
    *b = temp;
}

int64_t median_of_five(int64_t arr[]) {
    // Sort the first two and the next two elements
    if (arr[0] > arr[1]) swap(&arr[0], &arr[1]);
    if (arr[2] > arr[3]) swap(&arr[2], &arr[3]);

    // Place the smaller of each pair at the beginning
    if (arr[0] > arr[2]) swap(&arr[0], &arr[2]);
    if (arr[1] > arr[3]) swap(&arr[1], &arr[3]);

    // The smallest element is now in arr[0], and the largest is in arr[3] or arr[4].
    // We don't need them anymore for finding the median.

    // Compare and place the middle three elements
    if (arr[1] > arr[4]) swap(&arr[1], &arr[4]);
    if (arr[1] > arr[2]) swap(&arr[1], &arr[2]);
    if (arr[2] > arr[4]) swap(&arr[2], &arr[4]);

    // arr[2] is now the median of the five elements
    return arr[2];
}

int compare_int64(const void *a, const void *b) {
    return (*(int64_t*)a - *(int64_t*)b);
}

int64_t median_qsort(int64_t arr[]) {
    qsort(arr, 5, sizeof(int64_t), compare_int64);
    return arr[2];
}

int main() {
    const int trials = 100000;  // Reduced number of trials for debugging
    int64_t arr1[5], arr2[5];
    int success = 1;

    srand(time(NULL));

    for (int i = 0; i < trials; i++) {
        for (int j = 0; j < 5; j++) {
            arr1[j] = rand() % 100; // Smaller range for easy debugging
            arr2[j] = arr1[j];
        }

        int64_t median_custom = median_of_five(arr1);
        int64_t median_from_qsort = median_qsort(arr2);

        if (median_custom != median_from_qsort) {
            success = 0;
            printf("Discrepancy found in trial %d:\n", i);
            printf("Array: ");
            for (int k = 0; k < 5; k++) {
                printf("%" PRId64 " ", arr2[k]);
            }
            printf("\nMedian (custom): %" PRId64 "\n", median_custom);
            printf("Median (qsort): %" PRId64 "\n", median_from_qsort);
            break;
        }
    }

    if (success) {
        printf("Success: The medians match for all %d trials.\n", trials);
    } else {
        printf("Failure: There was a discrepancy in the medians.\n");
    }

    return 0;
}