#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variable to count key comparisons
int key_comparisons = 0;

// Function prototypes
void insertion_sort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);
void hybrid_merge_sort(int arr[], int left, int right, int threshold);
void generate_random_array(int arr[], int size, int max_value);

void insertion_sort(int arr[], int left, int right) {
    int i, j, key;
    for (i = left + 1; i <= right; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= left && arr[j] > key) {
            key_comparisons++;
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        if (j >= left) {
            key_comparisons++;
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        key_comparisons++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void hybrid_merge_sort(int arr[], int left, int right, int threshold) {
    if (left < right) {
        if ((right - left + 1) <= threshold) {
            insertion_sort(arr, left, right);
        } else {
            int mid = (left + right) / 2;

            hybrid_merge_sort(arr, left, mid, threshold);
            hybrid_merge_sort(arr, mid + 1, right, threshold);

            merge(arr, left, mid, right);
        }
    }
}

void generate_random_array(int arr[], int size, int max_value) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % max_value + 1; // Generate a random number in the range [1, max_value]
    }
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Define the fixed array size and maximum value for the random numbers
    int array_size = 1000000; // Fixed array size of 1 million
    int max_value = 1000000; // Largest number allowed in datasets

    // Define the range of thresholds to test
    int min_threshold = 1;
    int max_threshold = 100;

    // Open the CSV file for writing
    FILE *file = fopen("sorting_results_fixed_size.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return 1;
    }

    // Write CSV header
    fprintf(file, "Threshold,Key Comparisons,Time Taken (seconds)\n");

    // Test each threshold value
    for (int threshold = min_threshold; threshold <= max_threshold; threshold++) {
        int *arr = (int *)malloc(array_size * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Memory allocation failed for size %d!\n", array_size);
            fclose(file);
            return 1;
        }

        // Generate random data
        generate_random_array(arr, array_size, max_value);

        // Record start time
        clock_t start_time = clock();

        // Sort the array using hybrid merge sort
        key_comparisons = 0; // Reset key comparisons counter
        hybrid_merge_sort(arr, 0, array_size - 1, threshold);

        // Record end time
        clock_t end_time = clock();

        // Calculate time taken
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        // Write the results to the CSV file
        fprintf(file, "%d,%d,%f\n", threshold, key_comparisons, time_taken);

        // Free the allocated memory
        free(arr);
    }

    // Close the CSV file
    fclose(file);

    printf("Sorting results have been written to sorting_results_fixed_size.csv\n");

    return 0;
}
