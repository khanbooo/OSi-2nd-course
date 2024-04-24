#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void printError(char* funcName){
    printf("Unable to perform %s\n", funcName);
}

void myMkdir(char* path, mode_t mode){
    if (mkdir(path, mode) == -1){
        printError("myMkdir");
        return;
    }
}

void myLs(char* path){
    DIR* src = opendir(path);
    if (src == NULL){
        printError("myLs");
        return;
    }
    struct dirent* dirEntry;
    while (dirEntry = readdir(src)){
        printf("%s\n", dirEntry->d_name);
    }
}

void myRmDir(char* path){
    if (rmdir(path) == -1){
        printError("myRmDir");
        return;
    }
}

void myTouch(char* path, mode_t mode){
    printf("%o\n", mode);
    if (creat(path, mode) == -1){
        printError("myTouch");
        return;
    }
}

void myCat(char* path){
    int fd = open(path, O_RDONLY);
    if (fd == -1){
        printError("myCat");
        return;
    }
    char buffer[1024];
    int counter = 1;
    while (counter > 0){
        counter = read(fd, buffer, 1024 * sizeof(unsigned char));
        if (counter == -1){
            printError("myCat");
            close(fd);
            return;
        }
        int bWritten = 0;
        while (bWritten != counter){
            printf("%c", buffer[bWritten]);
            bWritten++;
        }
        //printf("\n");
    }
    close(fd);
}

void myRm(char* path){
    if (unlink(path) == -1){
        printError("myRm");
        return;
    }
}

void mySymlink(char* target, char* linkpath){
    if (symlink(target, linkpath) == -1){
        printError("mySymlink");
        return;
    }
}

void myReadLink(char* path){
    int bufsize = PATH_MAX;
    char buffer[bufsize];
    int nBytes = readlink(path, buffer, bufsize);
    if (nBytes == -1){
        printError("myReadLink");
        return;
    }
    buffer[nBytes] = '\0';
    for (int i = 0; i <= nBytes; i++){
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void myReadFileBySymlink(char* path){
    int bufsize = PATH_MAX;
    char buffer[bufsize];
    int nBytes = readlink(path, buffer, bufsize);
    if (nBytes == -1){
        printError("myReadFileBySymlink");
        return;
    }
    myCat(path);
}

void myLink(char* oldPath, char* newPath){
    if (link(oldPath, newPath) == -1){
        printError("myLink");
        return;
    }
}

// delete symlink == unlink => myRm already exists for this goals

void myStat(char* path){
    struct stat st;
    if (stat(path, &st) == -1){
        printError("myStat");
        return;
    }
    printf("mode: %o\nnumber of hard links: %ld\n", st.st_mode&0777, st.st_nlink);
}

void myChmod(char* path, mode_t mode){
    if (chmod(path, mode) == -1){
        printError("myChmod");
        return;
    }
}

int main(int argc, char** argv[]){
    if (argc != 2 && argc != 3){
        printf("bad arguments\n");
        return 0;
    }

    if (!strcmp(argv[0], "./myMkdir")){
        myMkdir(argv[1], strtol(argv[2], NULL, 8));
    }
    else if (!strcmp(argv[0], "./myLs")){
        myLs(argv[1]);
    }
    else if (!strcmp(argv[0], "./myRmDir")){
        myRmDir(argv[1]);
    }
    else if (!strcmp(argv[0], "./myTouch")){
        myTouch(argv[1], strtol(argv[2], NULL, 8));
    }
    else if (!strcmp(argv[0], "./myCat")){
        myCat(argv[1]);
    }
    else if (!strcmp(argv[0], "./myRm")){
        myRm(argv[1]);
    }
    else if (!strcmp(argv[0], "./mySymlink")){
        mySymlink(argv[1], argv[2]);
    }
    else if (!strcmp(argv[0], "./myReadLink")){
        myReadLink(argv[1]);
    }
    else if (!strcmp(argv[0], "./myReadFileBySymlink")){
        myReadFileBySymlink(argv[1]);
    }
    else if (!strcmp(argv[0], "./myLink")){
        myLink(argv[1], argv[2]);
    }
    else if (!strcmp(argv[0], "./myStat")){
        myStat(argv[1]);
    }
    else if (!strcmp(argv[0], "./myChmod")){
        myChmod(argv[1], strtol(argv[2], NULL, 8));
    }
    else if (!strcmp(argv[0], "./myUnHardlink")){
        myRm(argv[1]);
    }
    else if (!strcmp(argv[0], "./myUnSymlink")){
        myRm(argv[1]);
    }
}
