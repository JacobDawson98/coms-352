/* Compile with gcc posix-threads.c -o t.out -lm -pthread */

/* Author: Jacob Dawson */
/* COM S 352 Project 1 */

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

using namespace std;

extern void *sort(void *arguments);
int squaredTotal;

struct sort_arguments {
    int length;
    /* pass in row or col to work on */
    int** arr;
};


/* Given a file, open the file, if there is an error opening the file, exit. Else, rewind */
/* to the beginning of the file */
int _prepare_file(FILE *input) {
    if(!input) {
        printf("Error opening file.\n");
        return EXIT_FAILURE;
    }
    rewind(input);
    return 0;
}


/* Given a file, return the number of ints a file */
int get_num_ints(FILE *input) {
    _prepare_file(input);
    int temp = 0;
    int numItems = 0;
    while(fscanf(input, "%d", &temp) == 1) {
        ++numItems;
    }
    return numItems;
}


/* Given a file, empty 2d array, and the sqrt of the num vals to enter in the array, */
/* Populate the 2d array */
void populate_array(int* arr[squaredTotal], FILE *input) {
    _prepare_file(input);
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            fscanf(input, "%d", &arr[row][col]);
        }
    }
}


void swap(int *left, int *right) {
    int temp = *left;
    *left = *right;
    *right = temp;
}

/* Used to validate if a 2d array is sorted properly by shearsort standards. */
/* Parameters: */
/*     int dimension - the size of row or col */
/*     int arr[dimension][dimension] - the 2d square array to validate */
/* Returns: */
/*     bool - true if the array is sorted properly, false otherwise */
bool is_sorted(int* arr[squaredTotal]) {
    /* Are the columns sorted? */
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            if(arr[row][col] > arr[row + 1][col]) {
                return false;
            }
        }
    }
    /* Are the rows sorted? */
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal - 1; ++col) {
            if(row % 2 == 0) {
                if(arr[row][col] > arr[row][col + 1]) {
                    return false;
                }
            } else {
                if(arr[row][col] < arr[row][col + 1]) {
                    return false;
                }
            }
        }
    }
    return true;
}


/* sortType = 0 means sort by decreasing: greatest values -> smallest values */
/* sortType = 1 means sort by increasing: smallets values -> greatest values */
void *sort(void *arguments) {
    /* struct sort_arguments *args = (struct sort_arguments *)arguments; */
    /* int sortType = 0; */
    /* for(int i = 0 ; i < args -> length - 1; ++i) { */
    /*     for(int j = 0; j < args -> length - i - 1; ++j) { */
    /*         if(sortType == 0) { */
    /*             if(args -> arr[j] < args -> arr[j + 1]) { */
    /*                 swap(&args -> arr[j], &args -> arr[j + 1]); */
    /*             } */
    /*         } else { */
    /*             if(args -> arr[j] > args -> arr[j + 1]) { */
    /*                 swap(&args -> arr[j], &args -> arr[j + 1]); */
    /*             } */
    /*         } */
    /*     } */
    /* } */
    pthread_exit(NULL);
}

void print_array(int* arr[squaredTotal]) {
    printf("Squared total: %d\n", squaredTotal);
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    /* Populate 2d array from file */
    const char filename[] = "input.txt";
    FILE *input = fopen(filename, "r");
    squaredTotal = sqrt(get_num_ints(input));

    /* Allocate space for the 2d array */
    int** arrToSort= new int* [squaredTotal];
    for(int col = 0; col < squaredTotal; ++col) {
        arrToSort[col] = new int[squaredTotal];
    }

    populate_array(arrToSort, input);
    printf("Populated array: \n");
    print_array(arrToSort);

    /* Create n amount of threads */
    struct sort_arguments args;
    args.length = squaredTotal;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    pthread_t threads[squaredTotal];
    for(int curThread = 0; curThread < squaredTotal; ++curThread) {
        args.arr = arrToSort;
        if((pthread_create(&threads[curThread], NULL, &sort, (void *)&args)) != 0) {
            printf("Error creating thread number: %d\n", curThread);
            return EXIT_FAILURE;
        }
    }

    /* Sort until the array is sorted properly */
    while(!is_sorted(arrToSort)) {

    }

    return EXIT_SUCCESS;
}
