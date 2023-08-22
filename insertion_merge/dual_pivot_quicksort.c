#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// Inline swap function for swapping two elements
#define SWAP(a, b) { uint64_t temp = (a); (a) = (b); (b) = temp; }

// Partition the array using two pivot elements
void dualPivotPartition(uint64_t arr[], int low, int high, int* pivot1, int* pivot2) {
    // Ensure the lower pivot is smaller than the higher pivot
    if (arr[low] > arr[high]) {
        SWAP(arr[low], arr[high]);
    }

    int j = low + 1;
    int g = high - 1, k = low + 1;
    uint64_t p = arr[low], q = arr[high];

    while (k <= g) {
        if (arr[k] < p) {
            SWAP(arr[k], arr[j]);
            j++;
        } else if (arr[k] >= q) {
            while (arr[g] > q && k < g)
                g--;
            SWAP(arr[k], arr[g]);
            g--;
            if (arr[k] < p) {
                SWAP(arr[k], arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    SWAP(arr[low], arr[j]);
    SWAP(arr[high], arr[g]);

    *pivot1 = j;
    *pivot2 = g;
}

void dualPivotQuickSort(uint64_t arr[], int low, int high) {
    if (low < high) {
        int pivot1, pivot2;
        dualPivotPartition(arr, low, high, &pivot1, &pivot2);

        dualPivotQuickSort(arr, low, pivot1 - 1);
        dualPivotQuickSort(arr, pivot1 + 1, pivot2 - 1);
        dualPivotQuickSort(arr, pivot2 + 1, high);
    }
}

// Dual-pivot quicksort implementation
void dual_pivot_quicksort(uint64_t* L, uint64_t n) {
    dualPivotQuickSort(L, 0, n-1);
}