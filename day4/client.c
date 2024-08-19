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
#define IPV4 AF_INET
#define UDP SOCK_DGRAM

// pass ip address and port number as command line arguments
// <ip> <port>
int main(int argc, char* argv[]) {
    if(argc != 3){
        fprintf(stderr, "Usage: %s <ip_address> <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(IPV4, UDP, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = IPV4;
    servaddr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(IPV4, argv[1], &servaddr.sin_addr) <= 0) {
        perror("some error ");
        exit(EXIT_FAILURE);
    }
    int n, len;

    while (1) {
        printf("Client: ");
        scanf("%[^\n]%*c", buffer);
        buffer[strcspn(buffer, "\n")] = '\0'; 

        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        if (strcmp(buffer, "exit") == 0) {
            printf("Client has terminated the chat.\n");
            break;
        }

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Server has terminated the chat.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}