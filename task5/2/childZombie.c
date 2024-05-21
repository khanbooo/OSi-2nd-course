#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 5;

int main(int argc, char** argv){
    int b = 3;
    printf("global a = %d addr = %p, local b = %d addr = %p\n", a, &a, b, &b);
    printf("parent pid = %d\n", getpid());

    int pid;
    pid = fork();
    if (pid == 0){
        printf("child pid = %d\n", getpid());
    }
    sleep(15);

    if (pid == -1) {
        perror("fork failed\n");
        return 1;
    }
    else if (pid == 0){
        exit(5);
    }
    else if (pid > 0){
        printf("child-zombie pid = %d", pid);
        sleep(30);
    }
}