#include <sys/syscall.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

static ssize_t mywrite(int fd, const void *buf, size_t count){
  return syscall(SYS_write, fd, buf, count);
}

int main(){
  mywrite(1, "hello\n", 6);
  //write(1, "hello\n", 6);
//	printf("Hello world\n");
}

