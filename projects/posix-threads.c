/* Compile with gcc posix-threads.c -o t.out -lm -pthread */
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int _prepare_file(FILE *input) {
    if(!input) {
        printf("Error opening file.\n");
        return EXIT_FAILURE;
    }
    rewind(input);
    return 0;
}


int get_num_ints(FILE *input) {
    _prepare_file(input);
    int temp = 0;
    int numItems = 0;
    while(fscanf(input, "%d", &temp) == 1) {
        ++numItems;
    }
    return numItems;
}


void populate_array(int dimension, int arr[dimension][dimension], FILE *input) {
    _prepare_file(input);
    printf("Populated array: \n");
    for(int row = 0; row < dimension; ++row) {
        for(int col = 0; col < dimension; ++col) {
            fscanf(input, "%d", &arr[row][col]);
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}


void swap(int *left, int *right) {
    int temp = *left;
    *left = *right;
    *right = temp;
}


/* sortType = 0 means sort by decreasing: greatest values -> smallest values */
/* sortType = 1 means sort by increasing: smallets values -> greatest values */
void sort(int length, int arr[length], int sortType) {
    for(int i = 0 ; i < length - 1; ++i) {
        for(int j = 0; j < length - i - 1; ++j) {
            if(sortType == 0) {
                if(arr[j] < arr[j + 1]) {
                    swap(&arr[j], &arr[j + 1]);
                }
            } else {
                if(arr[j] > arr[j + 1]) {
                    swap(&arr[j], &arr[j + 1]);
                }
            }
        }
    }
}


int main(int argc, char *argv[]) {
    const char filename[] = "input.txt";
    FILE *input = fopen(filename, "r");
    int squaredTotal = sqrt(get_num_ints(input));
    int arrToSort[squaredTotal][squaredTotal];
    populate_array(squaredTotal, arrToSort, input);

    return EXIT_SUCCESS;
}
