#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define IPV4 AF_INET
#define TCP SOCK_STREAM

int main() {
    int sockfd;
    struct sockaddr_in serv;

    sockfd = socket(IPV4, TCP, 0);
    if (sockfd < 0) {
        printf("Socket creation error\n");
        exit(0);
    }

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        printf("Connection with the server failed\n");
        exit(0);
    }

    char buffer[1000];
    char res[1000];
    printf("Client : ");
    scanf("%s", res);
    write(sockfd, res, strlen(res));

    bzero(buffer, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));
    printf("From server: %s\n", buffer);

    close(sockfd);
    return 0;
}