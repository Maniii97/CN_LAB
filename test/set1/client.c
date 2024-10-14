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
    int sockfd;
    struct sockaddr_in serv;

    sockfd = socket(IPV4, TCP, 0);
    if (sockfd < 0) {
        printf("Socket creation error\n");
        exit(0);
    }

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = inet_addr("192.10.10.10");
    serv.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        printf("Connection with the server failed\n");
        close(sockfd);
        exit(0);
    }

    char buffer[1000];
    char res[1000];
    printf("Enter message to send to server: ");
    fgets(res, sizeof(res), stdin);
    write(sockfd, res, strlen(res));

    bzero(buffer, sizeof(buffer));
    int n = read(sockfd, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}