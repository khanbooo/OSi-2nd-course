#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void func() {
    char hw[12] = "hello world";

    char* p = (char*)malloc(100 * sizeof(char));
    memcpy(p, hw, 11);
    printf("p before free : %s\n", p);
    free(p);
    printf("p after free  : %s\n", p);

    char* p2 = (char*)malloc(100 * sizeof(char));
    memcpy(p2, hw, 11);
    printf("p2 before free: %s\n", p2);
    p2 += 50;
    free(p2);
    printf("p2 after free : %s\n", p2);
}

int main() {
    printf("pid = %d\n", getpid());
    func();
}