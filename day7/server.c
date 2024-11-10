#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3080
#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define EOF_MARKER "EOF"

int main() {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in serv, cli;
    char buffer[1024];
    int n;

    sockfd = socket(IPV4, TCP, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        perror("Server accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (system("ls *.c > a.txt") != 0) {
        perror("Failed to execute ls command");
        close(connfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen("a.txt", "r");
    if (file == NULL) {
        perror("File open error");
        close(connfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        write(connfd, buffer, strlen(buffer));
    }
    fclose(file);

    write(connfd, EOF_MARKER, strlen(EOF_MARKER));

    char chosen_file[256];
    bzero(chosen_file, sizeof(chosen_file));
    read(connfd, chosen_file, sizeof(chosen_file));
    chosen_file[strcspn(chosen_file, "\n")] = 0; 

    file = fopen(chosen_file, "r");
    if (file == NULL) {
        perror("File open error");
        close(connfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *ptr = buffer;
        while ((ptr = strstr(ptr, "int")) != NULL) {
            count++;
            ptr += 3;
        }
    }
    fclose(file);

    int network_count = htonl(count); 
    write(connfd, &network_count, sizeof(network_count));

    close(connfd);
    close(sockfd);
    return 0;
}
