#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>

#define PAGE_SIZE 4096

void sigSegvHandler(int sig){
    perror("SIGSEGV received");
    sleep(3);
    exit(1);
}

int main(int argc, char** argv) {
    printf("pid = %d\n", getpid());
    signal(SIGSEGV, sigSegvHandler);
    sleep(10);

    int block = 1024 * 10;
    int size = 0;

    int length = 10;
    int* arr[length];

    for (int i = 0; i < length; i++){
        arr[i] = malloc(block);
        size += block;
        printf("%d: size = %d || pid = %d\n", i, size, getpid());
//        mmap(NULL, 10 * PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        usleep(100000);
    }
    for (int i = 0; i < length; i++){
        free(arr[i]);
    }
    sleep(3);
    int* p = mmap(NULL, 10 * PAGE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("mapped 10 pages\n");
    sleep(15);

//    //allowed to read
//    mprotect(p, PAGE_SIZE * 10, PROT_READ);
//    //trying to write
//    p[0] = 1;
    printf("%p || %p\n", p, p + PAGE_SIZE * 3 / 4);

    munmap(p + PAGE_SIZE * 3 / 4, 3 * PAGE_SIZE);
    printf("unmapped 3 pages: 4-6\n");
    sleep(15);
    return 0;
}