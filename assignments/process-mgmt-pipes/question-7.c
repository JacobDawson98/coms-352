#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CHILD_PROCESS 0
#define P_READ 0
#define P_WRITE 1


int main(void) {
    int fileDesc[2], inputSumDesc[2], readBuffer, userInput;
    int sum = 0;
    pid_t pid;
    pipe(fileDesc);
    pipe(inputSumDesc);

    if((pid = fork()) == -1) {
        perror("fork");
        exit(0);
    }
    if(pid == CHILD_PROCESS) {
        close(fileDesc[P_READ]);
        printf("Enter one integer at a time. When you\'re done, enter -1.\n");
        scanf("%d", &userInput);
        write(fileDesc[P_WRITE], &userInput, sizeof(userInput));
        while(userInput != -1) {
            scanf("%d", &userInput);
            write(fileDesc[P_WRITE], &userInput, sizeof(userInput));
        }
        read(inputSumDesc[P_READ], &sum, sizeof(sum));
        printf("Received sum from parent in child process: %d.\n", sum);

        return EXIT_SUCCESS;
    } else {
        close(fileDesc[P_WRITE]);
        read(fileDesc[P_READ], &readBuffer, sizeof(readBuffer));
        while(readBuffer != -1) {
            sum += readBuffer;
            read(fileDesc[P_READ], &readBuffer, sizeof(readBuffer));
        }
        close(inputSumDesc[P_READ]);
        write(inputSumDesc[P_WRITE], &sum, sizeof(sum));

        return EXIT_SUCCESS;
    }
    return 0;
}
