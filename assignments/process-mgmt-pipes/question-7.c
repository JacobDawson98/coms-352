#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    int userInput, userInputDesc[2], inputSumDesc[2], readBuffer, sum;
    pid_t childpid;
    pipe(userInputDesc);
    pipe(inputSumDesc);

    childpid = fork();
    if(childpid == -1) {
        perror("fork");
        exit(0);
    }
    if(childpid == 0) { /* Child process */
        close(userInputDesc[0]);
        scanf("%d", &userInput);
        printf("Writing to pipe in child: %d\n", userInput);
        write(userInputDesc[1], &userInput, sizeof(userInput));
        while(userInput != -1) {
            scanf("%d", &userInput);
            printf("Writing to pipe in child: %d\n", userInput);
            write(userInputDesc[1], &userInput, sizeof(userInput));
        }
        exit(0);
    } else { /* Parent process */
        close(userInputDesc[1]);
        printf("In parent process\n");
        sum = 0;
        read(userInputDesc[0], &readBuffer, sizeof(readBuffer));
        while(readBuffer != -1) {
            sum += readBuffer;
            read(userInputDesc[0], &readBuffer, sizeof(readBuffer));
        }
        close(inputSumDesc[0]);
        printf("Writing to pipe in parent: %d\n", sum);
        write(inputSumDesc[1], &sum, sizeof(sum));
        exit(0);
    }
    return 0;
}
