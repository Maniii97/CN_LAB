#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define IPV4 AF_INET
#define UDP SOCK_DGRAM

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_to_send>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    int sockfd = socket(IPV4, UDP, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // fill server information
    servaddr.sin_family = IPV4;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int len = sizeof(cliaddr);
    int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';
    printf("Client request: %s\n", buffer);

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("File open failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while ((n = fread(buffer, 1, MAXLINE, file)) > 0) {
        if (sendto(sockfd, buffer, n, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len) < 0) {
            perror("sendto failed");
            fclose(file);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Sent %d bytes\n", n);
    }

    printf("File sent successfully.\n");

    fclose(file);
    close(sockfd);

    return 0;
}