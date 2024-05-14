#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

void buildPath(const char* path, const char* name, char* newDirectoryPath){
    strcpy(newDirectoryPath, path);
    if (path[strlen(path) - 1] != '/') {
        strcat(newDirectoryPath, "/");
    }
    strcpy(newDirectoryPath + strlen(path) + (path[strlen(path) - 1] != '/'), name);
}

void printError(char* funcName){
    fprintf(stderr, "Unable to perform %s:\n%s\n", funcName, strerror(errno));
}

void myChmod(char* path, mode_t mode){
    if (chmod(path, mode) == -1){
        fprintf(stderr, "Unable to change mode to %s\n", path);
        printError("myChmod");
        return;
    }
}

void myMkdir(char* path, mode_t mode){
    if (mkdir(path, 0777) == -1){
        fprintf(stderr, "Unable to create directory at %s\n", path);
        printError("myMkdir");
        return;
    }
    myChmod(path, mode);
}

void myLs(char* path){
    DIR* src = opendir(path);
    if (src == NULL){
        fprintf(stderr, "Unable to open directory at %s\n", path);
        printError("myLs");
        return;
    }
    struct dirent* dirEntry;
    while (dirEntry = readdir(src)){
        printf("%s\n", dirEntry->d_name);
    }
}

void myRm(char* path){
    if (unlink(path) == -1){
        fprintf(stderr, "Unable to delete file at %s\n", path);
        printError("myRm");
        return;
    }
}

void myRmDir(char* path){
    DIR* dir = opendir(path);
    struct dirent* dirEntry;
    if (dir == NULL){
        fprintf(stderr, "Unable to open directory at %s\n", path);
        printError("myRmDir");
        return;
    }
    while (dirEntry = readdir(dir)){
        if (dirEntry->d_type == DT_REG){
            char filePath[256];
            buildPath(path, dirEntry->d_name, filePath);
            myRm(filePath);
        }
        else if (dirEntry->d_type == DT_DIR && strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){
            char newSrc[256];
            buildPath(path, dirEntry->d_name, newSrc);
            myRmDir(newSrc);
        }
    }
    if (rmdir(path) == -1){
        fprintf(stderr, "Unable to delete directory at %s\n", path);
        printError("myRmDir");
        return;
    }
}

void myTouch(char* path, mode_t mode){
    printf("%o\n", mode);
    if (creat(path, 0777) == -1){
        fprintf(stderr, "Unable to create file at %s\n", path);
        printError("myTouch");
        return;
    }
    myChmod(path, mode);
}

void myCat(char* path){
    int fd = open(path, O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "Unable to open file at %s\n", path);
        printError("myCat");
        return;
    }
    char buffer[1024];
    long counter = 1;
    while (counter > 0){
        counter = read(fd, buffer, 1024 * sizeof(unsigned char));
        if (counter == -1){
            fprintf(stderr, "Error while reading from file\n");
            printError("myCat");
            close(fd);
            return;
        }
        int bWritten = 0;
        while (bWritten != counter){
            printf("%c", buffer[bWritten]);
            bWritten++;
        }
    }
    close(fd);
}

void mySymlink(char* target, char* linkPath){
    if (symlink(target, linkPath) == -1){
        fprintf(stderr, "Unable to create symbolic link to %s\n", target);
        printError("mySymlink");
        return;
    }
}

void myReadLink(char* path){
    int bufsize = PATH_MAX;
    char buffer[bufsize];
    long nBytes = readlink(path, buffer, bufsize);
    if (nBytes == -1){
        fprintf(stderr, "Unable to read link at %s\n", path);
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
    long nBytes = readlink(path, buffer, bufsize);
    if (nBytes == -1){
        fprintf(stderr, "Unable to read link at %s\n", path);
        printError("myReadFileBySymlink");
        return;
    }
    myCat(path);
}

void myLink(char* oldPath, char* newPath){
    if (link(oldPath, newPath) == -1){
        fprintf(stderr, "Unable to create link to %s\n", oldPath);
        printError("myLink");
        return;
    }
}

// delete symlink == unlink => myRm already exists for this goals

void myStat(char* path){
    struct stat st;
    if (stat(path, &st) == -1){
        fprintf(stderr, "Unable to get statistics of %s\n", path);
        printError("myStat");
        return;
    }
    printf("mode: %o\nnumber of hard links: %ld\n", st.st_mode&0777, st.st_nlink);
}

int main(int argc, char** argv){
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
