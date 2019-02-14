/* Compile with gcc question-5.c -o question-5 -pthread */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

extern void *world(void *arg);
extern void *hello(void *arg);
extern void *exclamation(void *arg);

pthread_t worldThread;
pthread_t helloThread;
pthread_t exclamationThread;

int main(int argc, char *argv[]) {
    pthread_create(&worldThread, NULL, world, NULL);
    pthread_create(&helloThread, NULL, hello, NULL);
    pthread_create(&exclamationThread, NULL, exclamation, NULL);
    pthread_join(exclamationThread, NULL);
    printf("\n");

    return EXIT_SUCCESS;
}

/* world - print the "world" part.*/
void *world(void *arg) {
    sched_yield();
    pthread_join(helloThread, NULL);
    printf("world");
    pthread_exit(NULL);
}

/* hello - print the "hello" part.*/
void *hello(void *arg) {
    printf("hello");
    pthread_exit(NULL);
}

/* exclamation – print “!”.*/
void  *exclamation(void *arg) {
    pthread_join(worldThread, NULL);
    printf("!");
    pthread_exit(NULL);
}
