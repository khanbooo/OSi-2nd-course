#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 10000

int main(){
    int srv_sock;
    struct sockaddr_in srv_sockaddr;
    struct sockaddr_in clt_sockaddr;
    int err;

    srv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (srv_sock == -1){
        printf("socket() failed %s\n", strerror(errno));
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_in));
    srv_sockaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &srv_sockaddr.sin_addr);
    srv_sockaddr.sin_port = htons(PORT);

    err = bind(srv_sock, (struct sockaddr *) &srv_sockaddr, sizeof(srv_sockaddr));
    if (err == -1){
        printf("bind() failed %s\n", strerror(errno));
        close(srv_sock);
        exit(1);
    }

    err = listen(srv_sock, 5);
    if (err == -1){
        printf("listen() failed %s\n", strerror(errno));
        close(srv_sock);
        exit(1);
    }

    printf("waiting for connections...\n");


    while(1){
        printf("accepting...\n");
        unsigned int len;
        int client = accept(srv_sock, (struct sockaddr *) &clt_sockaddr, &len);
        if (client == -1){
            perror(errno);
        }

        int client_pid;
        client_pid = fork();
        if (client_pid == 0){
            close(srv_sock);
            int buff_size = 4096;
            char buff[buff_size];
            while (1){
                int num_of_r_bytes = recv(client, buff, buff_size, 0);
                if (num_of_r_bytes == -1){
                    perror(errno);
                    exit(5);
                }
                if (num_of_r_bytes == 0) exit(5);
                printf("received message: %s\n", buff);
                int num_of_w_bytes = write(client, buff, buff_size);
                printf("message that was sent back: %s\n", buff);
            }
        }
    }
}