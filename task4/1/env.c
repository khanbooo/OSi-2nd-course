#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("pid = %d\n", getpid());

    char* env = getenv("test");
    printf("test env before setting: %s\n", env);
    setenv("test", "aaa", 1);
    env = getenv("test");
    printf("test env after setting : %s\n", env);
}