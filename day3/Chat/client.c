#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    while (1) {
        printf("Client: ");
        scanf("%[^\n]%*c", buffer);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send message to server
        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        if (strcmp(buffer, "bye") == 0) {
            printf("Client has terminated the chat.\n");
            break;
        }

        // Receive message from server
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        // Check if the received message is "bye"
        if (strcmp(buffer, "bye") == 0) {
            printf("Server has terminated the chat.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}