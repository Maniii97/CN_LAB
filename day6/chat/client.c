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
    char buffer[1000];
    int n;

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

    while (1) {
        bzero(buffer, sizeof(buffer));
        printf("Client: ");
        n = 0;
        while ((buffer[n++] = getchar()) != '\n');
        write(sockfd, buffer, strlen(buffer));

        bzero(buffer, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer));
        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}