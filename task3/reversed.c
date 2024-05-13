#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 buildPath(const char* path, const char* name, char* newDirPath, const int pathLen, const int nameLen);
 reverseString(char* src, char* dst);
 getDividedPath(const char* path, char* name, char* dirPath??, );
 reversedFileContent();
 copiedFileContent();
 etc.





 */

void buildPath(const char* path, const char* name, char* newDirectoryPath){
    strcpy(newDirectoryPath, path);
    if (path[strlen(path) - 1] != '/') {
        strcat(newDirectoryPath, "/");
    }
    strcpy(newDirectoryPath + strlen(path) + (path[strlen(path) - 1] != '/'), name);
}

void reverseString(const char* src, char* dst){
    unsigned int len = strlen(src);
    for (int i = 0; i < len; i++){
        dst[i] = src[len - i - 1];
    }
    dst[len] = '\0';
}

void myBaseName(char* path, char* baseName){
    int nameLength = 0;
    for (int i = 0; i < strlen(path); i++){
        if (path[strlen(path) - i - 1] == '/'){
            nameLength = i;
            break;
        }
    }
    for (int i = 0; i < nameLength; i++){
        baseName[i] = path[strlen(path) - nameLength + i];
    }
    baseName[nameLength] = '\0';
}

void reversedFileContent(int src, int dst, long src_size){
    unsigned char buffer[1024];
    unsigned char reversedBuffer[1024];

    lseek(src, src_size - src_size % 1024, SEEK_SET);
    long counter = 0;
    while (counter != src_size) {
        long tmp = read(src, buffer, 1024 * sizeof(unsigned char));
        counter += tmp;

        for (int i = 0; i < counter; i++) {
            reversedBuffer[i] = buffer[counter - i - 1];
        }

        long bCounter = 0;
        while (bCounter != tmp) {
            long bWritten = write(dst, reversedBuffer, tmp);
            if (bWritten == -1) {
                printf("bWritten == -1");
                return;
            }
            bCounter += bWritten;
        }

        if (counter + 1024 > src_size) {
            break;
        }
        lseek(src, src_size - (counter + 1024), SEEK_SET);
    }
}

void copiedFileContent(int src, int dst, long src_size){
    unsigned char buffer[1024];
    long counter = 0;
    while (counter != src_size) {
        long tmp = read(src, buffer, 1024 * sizeof(unsigned char));
        counter += tmp;

        long bCounter = 0;
        while (bCounter != tmp) {
            long bWritten = write(dst, buffer, tmp);
            if (bWritten == -1) {
                printf("bWritten == -1");
                return;
            }
            bCounter += bWritten;
        }
        lseek(src, counter, SEEK_SET);
    }
}

void makeReversedDir(char* src, char* dst){
    DIR* dir = opendir(src);
    struct dirent* dirEntry;

    if (dir == NULL){
        printf("Unable to open directory\n");
        return;
    }

    char dirName[256];
    myBaseName(src, dirName);

    char reversedDirName[256];
    reverseString(dirName, reversedDirName);

    char newDirectoryPath[256];
    buildPath(dst, reversedDirName, newDirectoryPath);

    printf("%s\n%s\n", reversedDirName, newDirectoryPath);

    mkdir(newDirectoryPath, 0777);
    int fdread = open(src, O_RDONLY);
    if (fdread == -1){
        printf("fdread == -1");
        return;
    }
    int fdwrite = open(newDirectoryPath, O_RDONLY);
    if (fdwrite == -1){
        printf("fdwrite == -1");
        close(fdread);
        return;
    }
    while (dirEntry = readdir(dir)){
        if (dirEntry->d_type == DT_REG){

            char reversedName[256];
            reverseString(dirEntry->d_name, reversedName);
            struct stat st;

            int filefdrd = openat(fdread, dirEntry->d_name, O_RDWR);
            if (filefdrd == -1){
                printf("filefdrd == -1");
                close(fdread);
                close(fdwrite);
                return;
            }
            fstatat(fdread, dirEntry->d_name, &st, 0);


            int filefdwr = openat(fdwrite, reversedName, O_CREAT | O_WRONLY, 0777);
            if (filefdwr == -1){
                printf("filefdwr == -1");
                close(fdread);
                close(fdwrite);
                close(filefdrd);
                return;
            }
            fchmodat(fdwrite, reversedName, st.st_mode, 0);

            reversedFileContent(filefdrd, filefdwr, st.st_size);
            close(filefdrd);
            close(filefdwr);
            close(fdread);
            close(fdwrite);
        }
        else if (dirEntry->d_type == DT_DIR && strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){
            char newSrc[256];
            buildPath(src, dirEntry->d_name, newSrc);
            printf("***\n%s\n%s\n***\n", newSrc, newDirectoryPath);
            makeReversedDir(newSrc, newDirectoryPath);
        }
    }
}

void copyDir(char* src, char* dst){
    DIR* dir = opendir(src);
    struct dirent* dirEntry;

    if (dir == NULL){
        printf("Unable to open directory\n");
        return;
    }

    char dirName[256];
    myBaseName(src, dirName);

    char newDirectoryPath[256];
    buildPath(dst, dirName, newDirectoryPath);

    printf("%s\n%s\n", dirName, newDirectoryPath);

    mkdir(newDirectoryPath, 0777);
    int fdread = open(src, O_RDONLY);
    if (fdread == -1){
        printf("fdread == -1");
        return;
    }
    int fdwrite = open(newDirectoryPath, O_RDONLY);
    if (fdwrite == -1){
        printf("fdwrite == -1");
        close(fdread);
        return;
    }
    while (dirEntry = readdir(dir)){
        if (dirEntry->d_type == DT_REG){
            struct stat st;

            int filefdrd = openat(fdread, dirEntry->d_name, O_RDWR);
            if (filefdrd == -1){
                printf("filefdrd == -1");
                close(fdread);
                close(fdwrite);
                return;
            }
            fstatat(fdread, dirEntry->d_name, &st, 0);


            int filefdwr = openat(fdwrite, dirEntry->d_name, O_CREAT | O_WRONLY, 0777);
            if (filefdwr == -1){
                printf("filefdwr == -1");
                close(fdread);
                close(fdwrite);
                close(filefdrd);
                return;
            }
            fchmodat(fdwrite, dirEntry->d_name, st.st_mode, 0);

            copiedFileContent(filefdrd, filefdwr, st.st_size);
            close(filefdrd);
            close(filefdwr);
            close(fdread);
            close(fdwrite);
        }
        else if (dirEntry->d_type == DT_DIR && strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")){
            char newSrc[256];
            buildPath(src, dirEntry->d_name, newSrc);
            printf("***\n%s\n%s\n***\n", newSrc, newDirectoryPath);
            copyDir(newSrc, newDirectoryPath);
        }
    }
}

int main(int argc, char** argv){
    if (argc != 3){
        return 1;
    }
    if (!strcmp(argv[0], "./makeReversedDir")){
        makeReversedDir(argv[1], argv[2]);
    }
    else if (!strcmp(argv[0], "./copyDir")){
        copyDir(argv[1], argv[2]);
    }
}