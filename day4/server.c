#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define PORT 8080
#define MAXLINE 1024
#define IPV4 AF_INET
#define UDP SOCK_DGRAM

// socket() -> bind() -> recvfrom() -> sendto() -> close()

int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <ip_address> <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    char *msg;
    int sockfd = socket(IPV4, UDP, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // fill server information
    servaddr.sin_family = IPV4;
    servaddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(IPV4, argv[1], &servaddr.sin_addr) <= 0) {
        perror("some error ");
        exit(EXIT_FAILURE);
    }

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

        char client_ip[INET_ADDRSTRLEN];
        int client_port = ntohs(cliaddr.sin_port);
        inet_ntop(IPV4, &(cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Client IP Address:  %s, CLient Port : %d\n", client_ip,client_port);
        printf("Client message: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client has terminated the chat.\n");
            break;
        }

        printf("Server: ");
        scanf("%[^\n]%*c", buffer);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(buffer, "exit") == 0) {
            sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            printf("Server has terminated the chat.\n");
            break;
        }

        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}