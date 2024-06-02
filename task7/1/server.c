#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 10000

int main(){
    int srv_sock;
    struct sockaddr_in srv_sockaddr;
    struct sockaddr_in clt_sockaddr;
    int err;

    srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (srv_sock == -1){
        printf("socket() failed %s\n", strerror(errno));
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
    srv_sockaddr.sin_family = AF_INET;
    srv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_sockaddr.sin_port = htons(PORT);

    err = bind(srv_sock, (struct sockaddr *) &srv_sockaddr, sizeof(srv_sockaddr));
    if (err == -1){
        printf("bind() failed %s\n", strerror(errno));
        close(srv_sock);
        exit(1);
    }

    int buff_size = 4096;
    char buff[buff_size];
    while(1){
        printf("listening...\n");
        unsigned int len;
        if (recvfrom(srv_sock, buff, buff_size, 0, (struct sockaddr *) &clt_sockaddr, &len) == -1){
            perror("recvfrom() failed");
            close(srv_sock);
            exit(1);
        }

        printf("received message: %s\n", buff);

        clt_sockaddr.sin_family = AF_INET;
        if (sendto(srv_sock, buff, buff_size, 0, (struct sockaddr *) &clt_sockaddr, sizeof(clt_sockaddr)) == -1) {
            perror("sendto() failed");
            close(srv_sock);
            exit(1);
        }

        printf("message that was sent back: %s\n", buff);
    }
}