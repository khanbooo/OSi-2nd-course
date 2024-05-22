#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 5;

int main(int argc, char** argv){
    int b = 3;
    printf("pid = %d\n", getpid());
    printf("global a = %d addr = %p, local b = %d addr = %p\n", a, &a, b, &b);
    int pid;
    pid = fork();

    if (pid == -1) {
        perror("fork failed\n");
        return 1;
    }
    else if (pid == 0){

        printf("parent pid = %d\n", getpid());
        int inner_pid = fork();

        if (inner_pid == 0){
            printf("child pid = %d\n", getpid());
        }
        sleep(15);

        if (inner_pid == -1){
            perror("fork failed\n");
            return 1;
        }
        else if (inner_pid == 0){
            printf("parent pid before: %d\n", getppid());
            sleep(6);
            printf("parent pid after: %d\n", getppid());
            sleep(30);
        }
        else if (inner_pid > 0){
            sleep(3);
            printf("parent-zombie pid = %d\n", getpid());
            printf("child pid = %d\n", inner_pid);
            exit(5);
        }
    }
    else if (pid > 0){
        sleep(60);
    }
}