/* Author: Jacob Dawson */
/* COM S 352 Project 1 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef vector<vector<int> > matrix;

extern void *sort(void *arguments);
int squaredTotal, numSorted, phase;
vector<bool> threadsStatus;
matrix matrixToSort;
pthread_mutex_t mutex_lock;
pthread_cond_t controlVar;

struct sort_arguments {
    long id;
    int length;
    vector<bool> boolVector;
    /* int** arr; */
    vector<vector<int> > matrix;
};


/* Given a file, open the file, if there is an error opening the file, exit. Else, rewind */
/* to the beginning of the file */
/* Parameters: */
    /* FILE *input - file to validate */
/* Returns: */
    /* 0 or EXIT_FAILURE, returns EXIT_FAILURE if an error occured */
/* int _prepare_file(string filename) { */
/*     if(!input) { */
/*         printf("Error opening file.\n"); */
/*         return EXIT_FAILURE; */
/*     } */
/*     rewind(input); */
/*     return 0; */
/* } */


/* Given a file, return the number of ints a file */
/* Parameters: */
    /* FILE *input - File to read values from */
/* Returns: */
    /* int numItems - The number of integers in the file */
int get_num_ints(string filename) {
    ifstream infile(filename);
    string currentLine;
    int numInts = 0;
    while(getline(infile, currentLine)) {
        istringstream iss(currentLine);
        int curInt;
        while(iss >> curInt) {
            ++numInts;
        }
    }
    return numInts;
}


/* Given a file, empty 2d array, and the sqrt of the num vals to enter in the array, */
/* Populate the 2d array */
/* Parameters: */
    /* int* arr[squaredTotal] - array to populate values from file */
    /* FILE* input - the opened file to read values from */
void populate_matrix(string filename, int width) {
    ifstream infile(filename);
    string currentLine;
    while(getline(infile, currentLine)) {
        istringstream iss(currentLine);
        int curInt;
        vector<int> newRow;
        while(iss >> curInt) {
            newRow.push_back(curInt);
        }
        matrixToSort.push_back(newRow);
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
bool is_sorted(matrix m) {
    /* Are the columns sorted? */
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            if(m[row][col] > m[row + 1][col]) {
                return false;
            }
        }
    }
    /* Are the rows sorted? */
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal - 1; ++col) {
            if(row % 2 == 0) {
                if(m[row][col] > m[row][col + 1]) {
                    return false;
                }
            } else {
                if(m[row][col] < m[row][col + 1]) {
                    return false;
                }
            }
        }
    }
    return true;
}


void sort_row(int rowToSort, matrix m) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++col) {
            if(rowToSort % 2 == 0) {
                if(m[rowToSort][col] > m[rowToSort][col + 1]) {
                    swap(m[rowToSort][col], m[rowToSort][col + 1]);
                }
            } else {
                if(m[rowToSort][col] < m[rowToSort][col + 1]) {
                    swap(m[rowToSort][col], m[rowToSort][col + 1]);
                }
            }
        }
    }
}


void sort_col(int colToSort, matrix m) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++row) {
            if(m[col][colToSort] > m[col + 1][colToSort]) {
                swap(m[col][colToSort], m[col][colToSort]);
            }
        }
    }
}


void *sort(void *arguments) {
    struct sort_arguments *args = (struct sort_arguments *)arguments;
    pthread_mutex_lock(&mutex_lock);
    cout << "This is thread id: " << args -> id << "\n";
    while(phase != 0) {
        printf("CALLING SORT FUNCTION\n");
        if(phase % 2 == 0) {
            sort_row(args -> id, args -> matrix);
        } else {
            sort_col(args -> id, args -> matrix);
        }
        cout << "modifying threadsStatus " << args -> id << "\n";
        threadsStatus[args -> id] = true;
        cout << "modified value" << threadsStatus[args -> id] << "\n";
        pthread_cond_wait(&controlVar, &mutex_lock);
    }
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
void print_matrix(matrix m) {
    matrix::iterator row;
    vector<int>::iterator col;
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            cout << m[row][col] << " ,";
        }
        cout << "\n";
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    /* Populate 2d array from file */
    string filename = "input.txt";
    squaredTotal = sqrt(get_num_ints(filename));

    /* Allocate space for the 2d array */
    populate_matrix(filename, squaredTotal);
    printf("Populated array: \n");
    print_matrix(matrixToSort);

    /* Create n amount of threads */
    threadsStatus.resize(squaredTotal);
    fill(threadsStatus.begin(), threadsStatus.end(), false);
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_cond_init(&controlVar, NULL);
    struct sort_arguments args;
    args.length = squaredTotal;
    pthread_t threads[squaredTotal]; // might not need, could use var in for loop
    for(long curThread = 0; curThread < squaredTotal; ++curThread) {
        args.matrix = matrixToSort;
        args.id = curThread;
        if((pthread_create(&threads[curThread], NULL, &sort, (void *)&args)) != 0) {
            /* printf("Error creating thread number: %d\n", curThread); */
            return EXIT_FAILURE;
        }
    }

    /* Sort until the array is sorted properly */
    phase = 1;
    while(!is_sorted(matrixToSort)) {
        numSorted = 0;
        while(!threads_complete(threadsStatus)) {
            /* print_vector(threadsStatus); */
        }
            pthread_mutex_lock(&mutex_lock);
            pthread_cond_broadcast(&controlVar);
            pthread_mutex_unlock(&mutex_lock);
        ++phase;
        printf("Phase %d\n", phase);
        print_matrix(matrixToSort);
        fill(threadsStatus.begin(), threadsStatus.end(), false);
        pthread_mutex_lock(&mutex_lock);
        pthread_cond_broadcast(&controlVar);
        pthread_mutex_unlock(&mutex_lock);
    }
    phase = 0;
    pthread_mutex_lock(&mutex_lock);
    pthread_cond_broadcast(&controlVar);
    pthread_mutex_unlock(&mutex_lock);
    for(int i = 0; i < squaredTotal; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_lock);

    return EXIT_SUCCESS;
}
