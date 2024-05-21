#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void myWrite(unsigned int* p){
    unsigned int max_num = PAGE_SIZE / sizeof(unsigned int);
    unsigned int curr_i = 0;
    while(1){
        for (unsigned int i = 0; i < max_num; i++){
            p[i] = curr_i++;
        }
    }
}

void myRead(unsigned int* p){
    sleep(2);
    unsigned int max_num = PAGE_SIZE / sizeof(unsigned int);
    unsigned int prev_i = 0;
    while(1){
        for (unsigned int i = 0; i < max_num; i++){
            unsigned int curr_i = p[i];
            if (prev_i + 1 != curr_i){
                if (prev_i == 0 && curr_i == 0){
                    continue;
                }
                printf("numbers are not consecutive\n"
                       "prev_i = %u\n"
                       "curr_i = %u\n",
                       prev_i, curr_i);
            }
            prev_i = curr_i;
        }
    }
}

int main(int argc, char** argv) {
    unsigned int* p = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    int pid;
    pid = fork();

    if (pid == -1){
        printf("fork failed\n");
        return -1;
    }
    else if (pid == 0){
        myRead(p);
    }
    else if (pid > 0){
        myWrite(p);
    }
    return 0;
}