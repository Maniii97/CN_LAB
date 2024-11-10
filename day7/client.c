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
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    int n;

    sockfd = socket(IPV4, TCP, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = IPV4;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection to the server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen("a.txt", "w");
    if (file == NULL) {
        perror("File open error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        if (strstr(buffer, EOF_MARKER) != NULL) {
            char *marker_pos = strstr(buffer, EOF_MARKER);
            *marker_pos = '\0'; 
            fprintf(file, "%s", buffer);
            break;
        }
        fprintf(file, "%s", buffer);
    }
    fclose(file);

    printf("File received\n");

    file = fopen("a.txt", "r");
    if (file == NULL) {
        perror("File open error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Files available:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);

    printf("Enter the name of the file to analyze: ");
    char chosen_file[256];
    fgets(chosen_file, sizeof(chosen_file), stdin);
    chosen_file[strcspn(chosen_file, "\n")] = 0; 

    write(sockfd, chosen_file, strlen(chosen_file) + 1);

    int network_count;
    read(sockfd, &network_count, sizeof(network_count));
    int int_count = ntohl(network_count);
    printf("The number of 'int' occurrences in %s: %d\n", chosen_file, int_count);

    close(sockfd);
    return 0;
}
