#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 10000

int main(){
    int clt_sock;
    int err;
    struct sockaddr_in srv_sockaddr;

    clt_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (clt_sock == -1){
        printf("socket() failed %s\n", strerror(errno));
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
    srv_sockaddr.sin_family = AF_INET;
    srv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_sockaddr.sin_port = htons(PORT);

    err = connect(clt_sock, (struct sockaddr *) &srv_sockaddr, sizeof(srv_sockaddr));
    if (err == -1){
        printf("connect() failed %s\n", strerror(errno));
        close(clt_sock);
        exit(1);
    }

    int buff_size = 4096;
    char buff[buff_size];

    while(1){
        memset(buff, 0, buff_size);
        printf("Enter a message you want to send: ");
        char* res = fgets(buff, buff_size, stdin);

        if (res == NULL) {
            perror("fgets() failed");
            close(clt_sock);
            exit(1);
        }

        if (buff[0] == '\n'){
            close(clt_sock);
            exit(1);
        }

        buff[strrchr(buff, '\n') - buff] = '\0';
        int num_of_w_bytes = write(clt_sock, buff, buff_size);
        printf("message sent: %s\n", buff);
        int num_of_r_bytes = read(clt_sock, buff, buff_size);
        printf("received message: %s\n", buff);
    }
}