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
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    int sockfd = socket(IPV4, UDP, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // fill server information
    servaddr.sin_family = IPV4;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Send initial message to server
    const char *initial_msg = "Requesting file";
    if (sendto(sockfd, initial_msg, strlen(initial_msg), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Initial message sent to server.\n");

    FILE *file = fopen("received_file.txt", "wb");
    if (file == NULL) {
        perror("File open failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int len = sizeof(servaddr);
    int n;

    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    printf("Received %d bytes\n", n);

    if (fwrite(buffer, 1, n, file) != n) {
        perror("File write failed");
        fclose(file);
        close(sockfd);
        exit(EXIT_FAILURE);
    }else{
        printf("Writing %d bytes\n", n);
    }

    if (n < 0) {
        perror("Receive failed");
    } else {
        printf("File received successfully.\n");
    }

    fclose(file);
    close(sockfd);

    return 0;
}