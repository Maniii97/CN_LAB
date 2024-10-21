#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3080
#define IPV4 AF_INET
#define TCP SOCK_STREAM

int main() {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in serv, cli;
    sockfd = socket(IPV4, TCP, 0);

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        printf("Error in bind");
        exit(0);
    }

    if (listen(sockfd, 5) != 0) {
        printf("Listen Error");
        exit(0);
    }

    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        printf("Failed accept");
        exit(0);
    }

    char buffer[1000];
    bzero(buffer, sizeof(buffer));
    int n = read(connfd, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);

    char *res = "Hi from server";
    write(connfd, res, strlen(res));

    close(connfd);
    close(sockfd);
}
