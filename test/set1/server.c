#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9090
#define IPV4 AF_INET
#define TCP SOCK_STREAM

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in serv, cli;
    sockfd = socket(IPV4, TCP, 0);

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = inet_addr("192.10.10.10");
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
    char* clientIp = inet_ntoa(cli.sin_addr);
    int clientPort = ntohs(cli.sin_port);
    char msg[n+1];
    for(int i=0;i<=n;i++){
        msg[i] = buffer[i];
    }
    printf("Client : %s\n", msg);
    printf("Client IP: %s\n", clientIp);
    printf("Client Port: %d\n", clientPort);

    write(connfd, msg, strlen(msg));

    close(connfd);
    close(sockfd);
}
