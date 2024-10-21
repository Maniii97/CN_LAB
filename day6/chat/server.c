#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_QUEUE 5

void handle_client(int connfd) {
    char buffer[1000];
    int n;
    while (1) {
        bzero(buffer, sizeof(buffer));
        n = read(connfd, buffer, sizeof(buffer));
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        if (strncmp(buffer, "logout", 6) == 0) {
            printf("Client requested logout.\n");
            break;
        }

        printf("Server: ");
        bzero(buffer, sizeof(buffer));
        n = 0;
        while ((buffer[n++] = getchar()) != '\n');
        write(connfd, buffer, strlen(buffer));
    }
    close(connfd);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }

    if ((listen(sockfd, MAX_QUEUE)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    printf("Server listening..\n");

    while (1) {
        len = sizeof(cliaddr);
        connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
        if (connfd < 0) {
            printf("Server accept failed...\n");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Accepted connection from %s:%d\n", client_ip, ntohs(cliaddr.sin_port));

        if (fork() == 0) {
            close(sockfd);
            handle_client(connfd);
            exit(0);
        } else {
            close(connfd);
        }
    }

    close(sockfd);
    return 0;
}