/* Author: Jacob Dawson */
/* COM S 352 Project 1 */

#include <iostream>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <vector>

using namespace std;

extern void *sort(void *arguments);
int squaredTotal, numSorted, phase;
vector<bool> threadsStatus;
pthread_mutex_t mutex_lock;

struct sort_arguments {
    int id;
    int length;
    vector<bool> boolVector;
    int** arr;
};


/* Given a file, open the file, if there is an error opening the file, exit. Else, rewind */
/* to the beginning of the file */
/* Parameters: */
    /* FILE *input - file to validate */
/* Returns: */
    /* 0 or EXIT_FAILURE, returns EXIT_FAILURE if an error occured */
int _prepare_file(FILE *input) {
    if(!input) {
        printf("Error opening file.\n");
        return EXIT_FAILURE;
    }
    rewind(input);
    return 0;
}


/* Given a file, return the number of ints a file */
/* Parameters: */
    /* FILE *input - File to read values from */
/* Returns: */
    /* int numItems - The number of integers in the file */
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
/* Parameters: */
    /* int* arr[squaredTotal] - array to populate values from file */
    /* FILE* input - the opened file to read values from */
void populate_array(int dimension, int** arr, FILE *input) {
    _prepare_file(input);
    for(int row = 0; row < dimension; ++row) {
        for(int col = 0; col < dimension; ++col) {
            fscanf(input, "%d", &arr[row][col]);
        }
    }
}


/* Swap two integers */
/* Parameters: */
/*     int *left - first of two integers to swap */
/*     int *right - second of two integers to swap */
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
bool is_sorted(int dimension, int** arr) {
    /* Are the columns sorted? */
    for(int row = 0; row < dimension - 1; ++row) {
        for(int col = 0; col < dimension; ++col) {
            if(arr[row][col] > arr[row + 1][col]) {
                return false;
            }
        }
    }
    /* Are the rows sorted? */
    for(int row = 0; row < dimension; ++row) {
        for(int col = 0; col < dimension - 1; ++col) {
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


void sort_row(int rowToSort, int** arr) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++col) {
            if(rowToSort % 2 == 0) {
                if(arr[rowToSort][col] > arr[rowToSort][col + 1]) {
                    swap(arr[rowToSort][col], arr[rowToSort][col + 1]);
                }
            } else {
                if(arr[rowToSort][col] < arr[rowToSort][col + 1]) {
                    swap(arr[rowToSort][col], arr[rowToSort][col + 1]);
                }
            }
        }
    }
}


void sort_col(int colToSort, int** arr) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++row) {
            if(arr[col][colToSort] > arr[col + 1][colToSort]) {
                swap(arr[col][colToSort], arr[col][colToSort]);
            }
        }
    }
}


void *sort(void *arguments) {
    printf("CALLING SORT FUNCTION"); // not printing
    struct sort_arguments *args = (struct sort_arguments *)arguments;
    pthread_mutex_lock(&mutex_lock);
    if(phase % 2 == 0) {
        sort_row(args -> id, args -> arr);
    } else {
        sort_col(args -> id, args -> arr);
    }
    threadsStatus[args -> id] = true;
    pthread_mutex_unlock(&mutex_lock);
    pthread_exit(NULL);
}


/* Returns true if all values in vector are true */
/* Parameters: */
/*     vector<bool> v - Vector to check values of */
/* Returns: */
/*     bool - true if all values are true, false otherwise */
bool threads_complete(vector<bool> v) {
    for(int i = 0; i < v.size(); ++i) {
        if(v[i] == false) {
            return false;
        }
    }
    return true;
}


void print_vector(vector<bool> v) {
    for(vector<bool>::const_iterator i = v.begin(); i!= v.end(); ++i) {
        cout << *i << ' ';
    }
}


/* Prints the values in a 2d array pictorially */
/* Parameters: */
/*     int* arr[squaredTotal] - The array to print the values of */
void print_array(int** arr) {
    printf("Squared total: %d\n", squaredTotal);
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }
    cout << "AT THE END OF PRINT ARRAY";
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

    populate_array(squaredTotal, arrToSort, input);
    printf("Populated array: \n");
    print_array(arrToSort);

    /* Create n amount of threads */
    threadsStatus.resize(pow(squaredTotal, 2));
    fill(threadsStatus.begin(), threadsStatus.end(), false);
    print_vector(threadsStatus);
    struct sort_arguments args;
    args.length = squaredTotal;
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_t threads[squaredTotal]; // might not need, could use var in for loop
    for(int curThread = 0; curThread < squaredTotal; ++curThread) {
        args.arr = arrToSort;
        args.id = curThread;
        cout << "\nRESIZED THREADSTATUS";
        cout << "\n";
        if((pthread_create(&threads[curThread], NULL, &sort, (void *)&args)) != 0) {
            printf("Error creating thread number: %d\n", curThread);
            return EXIT_FAILURE;
        }
    }

    /* Sort until the array is sorted properly */
    phase = 0;
    while(!is_sorted(squaredTotal, arrToSort)) {
        numSorted = 0;
        while(!threads_complete(threadsStatus)) {}
        ++phase;
        printf("Phase %d\n", phase);
        print_array(arrToSort);
        fill(threadsStatus.begin(), threadsStatus.end(), false);
    }
    pthread_mutex_destroy(&mutex_lock);

    return EXIT_SUCCESS;
}
