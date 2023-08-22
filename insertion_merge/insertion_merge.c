#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

#define MAX_LOG_SIZE 64
#define MAX_SWAPS 30

uint64_t* buffer_ptr_array[MAX_LOG_SIZE] = {0};
uint64_t  buffer_ptr_array_size = 0;

void print_value(uint64_t x) {
    printf("%" PRIu64, x);
}


void print_list(uint64_t* L, uint64_t n) {
    printf("[");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRIu64 ", ", L[i]);
    }
    printf("]");
}

void print_EOL() {
    printf("\n");
    fflush(stdout);
}

// Function to print a buffer of uint64_t values with length
void print_buffer(uint64_t* buffer) {
    printf("(");
    if (buffer == NULL) {
        printf("Buffer is NULL");
    } else {
        uint64_t length = buffer[0];
        for (uint64_t i = 0; i < length; i++) {
            printf("%" PRIu64 " ", buffer[i + 1]);
        }
    }
    printf(")");
}

uint64_t floor_log2(uint64_t n) {

    uint64_t result = 0;
    while (n > 1) {
        n >>= 1; // Right-shift the number by 1 (equivalent to division by 2)
        result++;
    }
    return result;

}

// Function to merge two sorted lists and return a new buffer
uint64_t* merge(uint64_t* fst_list, uint64_t fst_len, uint64_t* snd_list, uint64_t snd_len) {

    // Allocate memory for the merged buffer
    uint64_t* merged_buffer = (uint64_t*)malloc(sizeof(uint64_t) * (fst_len + snd_len + 1));
    
    // Set the length of the merged buffer
    merged_buffer[0] = fst_len + snd_len;

    // Initialize iterators
    uint64_t i = 0, j = 0, k = 1;

    // Merge the two sorted lists into the merged buffer
    while (i < fst_len && j < snd_len) {
        // Compare elements and add the smaller one to the merged buffer
        if (fst_list[i] <= snd_list[j]) {
            merged_buffer[k++] = fst_list[i++];
        } else {
            merged_buffer[k++] = snd_list[j++];
        }
    }
    
    // Copy any remaining elements from the first list
    while (i < fst_len) {
        merged_buffer[k++] = fst_list[i++];
    }
    
    // Copy any remaining elements from the second list
    while (j < snd_len) {
        merged_buffer[k++] = snd_list[j++];
    }
    
    // Return the merged buffer
    return merged_buffer;

}

void merge_sorted_portion_into_buffer_ptr_array(uint64_t* L, uint64_t i, uint64_t sorted_portion_len) {

    // Find the logarithmic length of the sorted portion
    uint64_t logarithmic_length = floor_log2(sorted_portion_len >> 4);
    
    if (logarithmic_length >= buffer_ptr_array_size) {
        buffer_ptr_array_size = logarithmic_length + 1;
    }

    // Get the relevant buffers
    uint64_t* sorted_portion = L + i - sorted_portion_len;
    uint64_t* next_buffer = buffer_ptr_array[logarithmic_length];

    if (next_buffer == NULL) {

        // Save the sorted portion into a new buffer
        buffer_ptr_array[logarithmic_length] = (uint64_t*) malloc((1 + sorted_portion_len) * sizeof(uint64_t));
        buffer_ptr_array[logarithmic_length][0] = sorted_portion_len;
        memcpy(buffer_ptr_array[logarithmic_length] + 1, sorted_portion, sorted_portion_len * sizeof(uint64_t));

    } else {

        // Merge the sorted portion with the buffer
        uint64_t  next_buffer_len = next_buffer[0];
        uint64_t* curr_buffer = merge(sorted_portion, sorted_portion_len, next_buffer + 1, next_buffer_len);
        uint64_t  curr_buffer_len = curr_buffer[0];
        free(next_buffer);
        buffer_ptr_array[logarithmic_length] = NULL;

        // Deal with the possibility that we're not allowed to store the array at logarithmic_length + 1
        uint64_t j = logarithmic_length + 1;
        for (; j<buffer_ptr_array_size; j++) {
            next_buffer = buffer_ptr_array[j];
            if (next_buffer == NULL) {
                break;
            }
            next_buffer_len = next_buffer[0];
            uint64_t* new_buffer = merge(curr_buffer + 1, curr_buffer_len, next_buffer + 1, next_buffer_len);
            free(curr_buffer);
            free(next_buffer);
            buffer_ptr_array[j] = NULL;
            curr_buffer = new_buffer;
            curr_buffer_len = curr_buffer[0];
        }

        // Store the buffer
        buffer_ptr_array[j] = curr_buffer;
        if (j >= buffer_ptr_array_size) {
            buffer_ptr_array_size = j + 1;
        }

    }

}

// Main sorting function
void insertion_merge(uint64_t* L, uint64_t len) {

    // Reset the size of the buffer_ptr_array
    buffer_ptr_array_size = 0;
    
    // Initialize variables
    uint64_t sorted_portion_len = 1;
        
    // Loop through the array
    for (uint64_t i = 1; i < len; i++) {

        // Get the value we're trying to insert
        uint64_t value = L[i];

        // Find the number of swaps needed to insert L[i] into the sorted portion
        uint64_t swaps_needed = 0; 
        uint64_t j = i - 1;
        while (j != UINT64_MAX && value < L[j] && i - j <= sorted_portion_len) {
            swaps_needed++;
            if (swaps_needed > MAX_SWAPS) {
                break; // Exit the loop if swaps_needed exceeds MAX_SWAPS
            }
            j--;
        }
        
        // Check if swaps_needed exceeds MAX_SWAPS
        if (swaps_needed <= MAX_SWAPS) {

            // Insert the item
            j = i - 1;
            while (j != UINT64_MAX && (int64_t) j >= (int64_t) i - (int64_t) swaps_needed) {
                L[j + 1] = L[j];
                j--;
            }
            L[j + 1] = value;
            sorted_portion_len++;

        } else {

            // Put the sorted portion into the relevant spot in the buffer array, merging if necessary
            merge_sorted_portion_into_buffer_ptr_array(L, i, sorted_portion_len);

            // Reset the length of the sorted portion
            sorted_portion_len = 1;

        }

    }

    // If we didn't use any buffers, the list is now sorted, and we can just return here
    if (buffer_ptr_array_size == 0) {
        return;
    }

    // Otherwise, insert the remaining segment of L into buffer_ptr_array
    merge_sorted_portion_into_buffer_ptr_array(L, len, sorted_portion_len);

    // Flatten buffer_ptr_array down to a single buffer
    uint64_t* curr_buffer; uint64_t curr_buffer_len;
    uint64_t* next_buffer; uint64_t next_buffer_len;
    uint64_t j = 0;
    for (; j<buffer_ptr_array_size; j++) {
        curr_buffer = buffer_ptr_array[j];
        if (curr_buffer != NULL) {
            buffer_ptr_array[j] = NULL;
            curr_buffer_len = curr_buffer[0];
            j++;
            break;
        }
    }
    for (; j<buffer_ptr_array_size; j++) {
        next_buffer = buffer_ptr_array[j];
        if (next_buffer == NULL) {
            continue;
        }
        buffer_ptr_array[j] = NULL;
        next_buffer_len = next_buffer[0];
        uint64_t* new_buffer = merge(curr_buffer + 1, curr_buffer_len, next_buffer + 1, next_buffer_len);
        free(curr_buffer);
        free(next_buffer);
        curr_buffer = new_buffer;
        curr_buffer_len = curr_buffer[0];
    }
    memcpy(L, curr_buffer + 1, curr_buffer_len * sizeof(uint64_t));
}