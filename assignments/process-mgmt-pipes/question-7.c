#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CHILD_PROCESS 0
#define P1_READ 0
#define P2_READ 1
#define P1_WRITE 2
#define P2_WRITE 3
#define NUM_PIPES 2


int main(void) {
    int fileDesc[NUM_PIPES], inputSumDesc[NUM_PIPES], readBuffer, userInput;
    int sum = 0;
    pid_t pid;
    pipe(fileDesc);
    pipe(inputSumDesc);

    pid = fork();
    if(pid == -1) {
        perror("fork");
        exit(0);
    }
    if(pid == CHILD_PROCESS) { /* Child process */
        close(fileDesc[0]);
        scanf("%d", &userInput);
        printf("Writing to pipe in child: %d\n", userInput);
        write(fileDesc[1], &userInput, sizeof(userInput));
        while(userInput != -1) {
            scanf("%d", &userInput);
            printf("Writing to pipe in child: %d\n", userInput);
            write(fileDesc[1], &userInput, sizeof(userInput));
        }
        read(inputSumDesc[0], &sum, sizeof(sum));
        printf("Received sum in child: %d\n", sum);
        return EXIT_SUCCESS;
    } else { /* Parent process */
        close(fileDesc[1]);
        printf("In parent process\n");
        read(fileDesc[0], &readBuffer, sizeof(readBuffer));
        while(readBuffer != -1) {
            sum += readBuffer;
            read(fileDesc[0], &readBuffer, sizeof(readBuffer));
        }
        close(inputSumDesc[0]);
        printf("Writing to pipe in parent: %d\n", sum);
        write(inputSumDesc[1], &sum, sizeof(sum));
        return EXIT_SUCCESS;
    }
    return 0;
}
