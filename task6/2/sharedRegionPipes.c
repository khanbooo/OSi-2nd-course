#include <stdio.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void myWrite(int fd){
    unsigned int max_num = PAGE_SIZE / sizeof(unsigned int);
    unsigned int curr_i = 0;
    while(1){
        for (unsigned int i = 0; i < max_num; i++){
            write(fd, &curr_i, sizeof(curr_i));
            ++curr_i;
        }
    }
}

void myRead(unsigned int fd){
    sleep(2);
    unsigned int max_num = PAGE_SIZE / sizeof(unsigned int);
    unsigned int prev_i = 0;
    while(1){
        for (unsigned int i = 0; i < max_num; i++){
            unsigned int curr_i;
            read(fd, &curr_i, sizeof(curr_i));
            if (prev_i + 1 != curr_i){
                if (prev_i == 0 && curr_i == 0){
                    continue;
                }
                printf("numbers are not consecutive\n"
                       "prev_i = %u\n"
                       "p[i] = %u\n",
                       prev_i, curr_i);
            }
            prev_i = curr_i;
        }
    }
}

int main(int argc, char** argv) {
    int pipes[2];

    if (pipe(pipes)){
        printf("pipe failed\n");
        return -1;
    }

    int pid;
    pid = fork();

    if (pid == -1){
        printf("fork failed\n");
        return -1;
    }
    else if (pid == 0){
        myRead(pipes[0]);
    }
    else if (pid > 0){
        myWrite(pipes[1]);
    }

    close(pipes[0]);
    close(pipes[1]);
    return 0;
}