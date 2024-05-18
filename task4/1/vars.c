#include <stdio.h>
#include <unistd.h>

int ga;
int gb = 10;
const int gc = 3;

void func() {
    int a = 5;
    static int b = 3;
    const int c = 4;

    printf("address of a = %d: %p\n"
           "address of b = %d: %p\n"
           "address of c = %d: %p\n"
           "address of ga: %p\n"
           "address of gb = %d: %p\n"
           "address of gc = %d: %p\n",
           a, &a, b, &b, c, &c, &ga, gb, &gb, gc, &gc);
}

int main() {
    printf("pid = %d\n", getpid());
    func();
}