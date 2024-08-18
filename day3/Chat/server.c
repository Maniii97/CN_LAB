#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define PORT     8080
#define MAXLINE 1024

// socket() -> bind() -> recvfrom() -> sendto() -> close()

int main(int argc, char* argv[]){
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    char *msg;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // bind the socket with the server address
    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    int len, n; // len is value/result, n is the number of bytes received
    len = sizeof(cliaddr);

    while(1) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "bye") == 0) {
            printf("Client has terminated the chat.\n");
            break;
        }

        printf("Server: ");
        scanf("%[^\n]%*c", buffer);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(buffer, "bye") == 0) {
            sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            printf("Server has terminated the chat.\n");
            break;
        }

        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}