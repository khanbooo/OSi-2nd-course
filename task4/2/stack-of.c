#include <stdio.h>
#include <unistd.h>
int i = 0;

void stackOverFlow(){
    int array[4096];
    printf("%d: pid = %d\n", i++, getpid());
    usleep(100000);
    stackOverFlow();
}

int main(int argc, char** argv) {
    printf("pid = %d\n", getpid());
    sleep(10);
    stackOverFlow();
    return 0;
}