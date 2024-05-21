#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


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
        printf("CHILD pid = %d, ppid = %d\n", getpid(), getppid());
        printf("CHILD global a = %d addr = %p, local b = %d addr = %p\n", a, &a, b, &b);
        a = 10;
        b = 11;
        printf("CHILD global a = %d addr = %p, local b = %d addr = %p\n", a, &a, b, &b);
        exit(5);
    }
    else if (pid > 0){
        printf("PARENT global a = %d addr = %p, local b = %d addr = %p\n", a, &a, b, &b);
        sleep(3);
        int status;
        waitpid(pid, &status, 0);
        printf("exit status: %d\n", WEXITSTATUS(status));
        sleep(10);
    }
}