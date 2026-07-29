#include <stdio.h>
#include <stdlib.h>

// Helper comparison function for qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}
int subarrayBitwiseORs(int* arr, int arrSize) {
    if (arrSize == 0) return 0;

    // 'current_or' stores unique OR values ending at the current index.
    // 'next_or' stores unique OR values for the next index.
    // Bound is 32 because integers have at most 30-31 bits that can change.
    int current_or[33]; 
    int next_or[33];
    int cur_size = 0;

    // Temporary flat array to collect all unique global results
    // Max size bounded safely by 32 * arrSize
    int max_possible_results = 32 * arrSize;
    int* all_results = (int*)malloc(max_possible_results * sizeof(int));
    int result_count = 0;

    for (int i = 0; i < arrSize; i++) {
        int next_size = 0;
        
        // 1. Every subarray can start fresh with the element itself
        next_or[next_size++] = arr[i];
        all_results[result_count++] = arr[i];

        // 2. Accumulate OR values with previous subarray results
        for (int j = 0; j < cur_size; j++) {
            int new_or = current_or[j] | arr[i];
            
            // Avoid adding duplicates into the immediate next sequence
            if (next_or[next_size - 1] != new_or) {
                next_or[next_size++] = new_or;
                all_results[result_count++] = new_or;
            }
        }

        // Swap or copy next_or over to current_or for the next iteration
        cur_size = next_size;
        for (int j = 0; j < cur_size; j++) {
            current_or[j] = next_or[j];
        }
    }

    // 3. Find unique values across all collected results
    qsort(all_results, result_count, sizeof(int), compare);

    int unique_count = 0;
    if (result_count > 0) {
        unique_count = 1;
        for (int i = 1; i < result_count; i++) {
            if (all_results[i] != all_results[i - 1]) {
                all_results[unique_count++] = all_results[i];
            }
        }
    }

    free(all_results);
    return unique_count;
}
