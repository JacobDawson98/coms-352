#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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
    printf("The number of items in the file is: %d\n", numItems);
    return numItems;
}


void populate_array(int dimension, int arr[dimension][dimension], FILE *input) {
    _prepare_file(input);
    int row, col;
    for(row = 0; row < dimension; ++row) {
        for(col = 0; col < dimension; ++col) {
            fscanf(input, "%d", &arr[row][col]);
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {
    const char filename[] = "input.txt";
    FILE *input = fopen(filename, "r");
    int squaredTotal = sqrt(get_num_ints(input));
    printf("Square root of total items is: %d\n\n", squaredTotal);
    int arrToSort[squaredTotal][squaredTotal];
    populate_array(squaredTotal, arrToSort, input);
    return EXIT_SUCCESS;
}

