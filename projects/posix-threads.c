/* Compile with gcc posix-threads.c -o t.out -lm -pthread */
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

extern void *sort(void *arguments);

struct sort_arguments {
    int length;
    int arr[];
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


/* Given a 2d array, return a 1d array that has the same values of the 2d array */
int get_array(int dimension, int val[dimension][dimension]) {
   int arr[(int)pow(dimension, 2)];
   int arrCounter = 0;
   for(int i = 0; i < dimension; ++i) {
       for(int j = 0; j < dimension; ++j) {
            arr[arrCounter] = val[i][j];
            ++arrCounter;
       }
   }
   return *arr;
}


void swap(int *left, int *right) {
    int temp = *left;
    *left = *right;
    *right = temp;
}


/* sortType = 0 means sort by decreasing: greatest values -> smallest values */
/* sortType = 1 means sort by increasing: smallets values -> greatest values */
void *sort(void *arguments) {
    struct sort_arguments *args = (struct sort_arguments *)arguments;
    int sortType = 0;
    for(int i = 0 ; i < args -> length - 1; ++i) {
        for(int j = 0; j < args -> length - i - 1; ++j) {
            if(sortType == 0) {
                if(args -> arr[j] < args -> arr[j + 1]) {
                    swap(&args -> arr[j], &args -> arr[j + 1]);
                }
            } else {
                if(args -> arr[j] > args -> arr[j + 1]) {
                    swap(&args -> arr[j], &args -> arr[j + 1]);
                }
            }
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    /* Populate 2d array from file */
    const char filename[] = "input.txt";
    FILE *input = fopen(filename, "r");
    int squaredTotal = sqrt(get_num_ints(input));
    int arrToSort[squaredTotal][squaredTotal];
    populate_array(squaredTotal, arrToSort, input);

    /* Create n amount of threads */
    struct sort_arguments args;
    args.length = squaredTotal;
    *args.arr = get_array(squaredTotal, arrToSort);
    for(int numThreads = 0; numThreads < squaredTotal; ++numThreads) {
        pthread_t curThread;
        if((pthread_create(&curThread, NULL, &sort, (void *)&args)) != 0) {
            printf("Error creating thread number: %d\n", numThreads);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
