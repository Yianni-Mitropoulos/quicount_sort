#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

void swap(int64_t *a, int64_t *b) {
    int64_t temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int64_t *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapsort(int64_t *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int compare_int64(const void *a, const void *b) {
    return (*(int64_t*)a - *(int64_t*)b);
}

void fill_random(int64_t *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand(); // Fill the array with random 64-bit integers
}

int partition(int64_t *arr, int low, int high) {
    int pivotIndex = low + rand() % (high - low);
    int64_t pivot = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[high]);
    int i = low;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

void quickheapsort(int64_t *arr, int low, int high) {
    if (low < high) {
        if (high - low <= 256/*65536/4096*/) {
            heapsort(arr + low, high - low + 1);
        } else {
            int pi = partition(arr, low, high);
            quickheapsort(arr, low, pi - 1);
            quickheapsort(arr, pi + 1, high);
        }
    }
}

int main() {
    const int n = 1024;
    int64_t *arr1 = malloc(n * sizeof(int64_t));
    int64_t *arr2 = malloc(n * sizeof(int64_t));
    int64_t *arr3 = malloc(n * sizeof(int64_t));

    if (!arr1 || !arr2 || !arr3) {
        printf("Memory allocation failed\n");
        free(arr1);
        free(arr2);
        free(arr3);
        return 1;
    }

    srand(time(NULL)); // Seed the random number generator

    fill_random(arr1, n);
    memcpy(arr2, arr1, n * sizeof(int64_t)); // Duplicate the array
    memcpy(arr3, arr1, n * sizeof(int64_t)); // Duplicate the array for quickheapsort

    clock_t start_heapsort = clock();
    heapsort(arr1, n);
    clock_t end_heapsort = clock();

    clock_t start_qsort = clock();
    qsort(arr2, n, sizeof(int64_t), compare_int64);
    clock_t end_qsort = clock();

    clock_t start_quickheapsort = clock();
    quickheapsort(arr3, 0, n - 1);
    clock_t end_quickheapsort = clock();

    double time_heapsort = (double)(end_heapsort - start_heapsort) / CLOCKS_PER_SEC;
    double time_qsort = (double)(end_qsort - start_qsort) / CLOCKS_PER_SEC;
    double time_quickheapsort = (double)(end_quickheapsort - start_quickheapsort) / CLOCKS_PER_SEC;

    printf("Heapsort took %f seconds\n", time_heapsort);
    printf("qsort took %f seconds\n", time_qsort);
    printf("Quickheapsort took %f seconds\n", time_quickheapsort);

    // Check if the arrays sorted by heapsort, quickheapsort, and qsort are equal
    if (memcmp(arr1, arr2, n * sizeof(int64_t)) == 0 && memcmp(arr1, arr3, n * sizeof(int64_t)) == 0) {
        printf("All sorting algorithms produced the same result.\n");
    } else {
        printf("There was a discrepancy in the sorting results.\n");
    }

    free(arr1);
    free(arr2);
    free(arr3);
    return 0;
}
