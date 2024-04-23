#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void buildPath(char* path, char* name, char* newDirPath,
               const unsigned int* len, const unsigned int* nameLength){
    strcpy(newDirPath, path);
    strcpy(newDirPath + *len - *nameLength, name);
}

void reverseString(char* src, char* dst){
    unsigned int len = strlen(src);
    for (int i = 0; i < len; i++){
        dst[i] = src[len - i - 1];
    }
    dst[len] = '\0';
}

void getDividedPath(const char* path, char* name, char* dirPath,
                    const unsigned int* len, unsigned int* counter){
    for (; (*len - *counter - (int)1) >= 0; (*counter)++){
        if (path[*len - *counter - 1] == '/'){
            name[*counter] = '\0';
            break;
        }
        name[*counter] = path[*len - *counter - 1];
    }
    for (int i = 0; i < *len - *counter; i++){
        dirPath[i] = path[i];
    }
    dirPath[*len - *counter] = '\0';
}

void reversedFileContent(int src, int dst, int src_size){
  unsigned char buffer[1024];
  unsigned char reversedBuffer[1024];

  lseek(src, src_size - src_size % 1024, SEEK_SET);
  int counter = 0;
  while (counter != src_size) { 
    int tmp = read(src, buffer, 1024 * sizeof(unsigned char));
    counter += tmp;

    for (int i = 0; i < counter; i++) {
       reversedBuffer[i] = buffer[counter - i - 1];
    }

    int bCounter = 0; 
    while (bCounter != tmp) {
        int bWritten = write(dst, reversedBuffer, tmp);
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

void makeReversedDir(char* path){
  DIR* src = opendir(path);
  struct dirent* dirEntry;
  if (src == NULL){
    printf("Unable to open directory\n");
    return;
  }
  unsigned int len = strlen(path);
  unsigned int nameLength = 0;
  char reversedName[256];
  char dirPath[256];
  getDividedPath(path, reversedName, dirPath, &len, &nameLength);

  char newDirPath[256];
  buildPath(dirPath, reversedName, newDirPath, &len, &nameLength);

  mkdir(newDirPath, 0777);
  int fdread = open(path, O_RDONLY);
  if (fdread == -1){
    printf("fdread == -1");
    return;
  }
  int fdwrite = open(newDirPath, O_RDONLY);
  if (fdwrite == -1){
    printf("fdwrite == -1");
    close(fdread);
    return;
  }
  while (dirEntry = readdir(src)){
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
  }
}

int main(int argc, char** argv){
    printf("%d\n", argc);
    if (argc != 2){
        return 1;
    }
    makeReversedDir(argv[1]);
}
