#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define PORT	 8080
#define MAXLINE 1024
#define IPV4 AF_INET
#define UDP SOCK_DGRAM

// socket() -> sendto() -> recvfrom() -> close()

int main(){
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    char *msg = "Hello from client";
    int sockfd = socket(IPV4, UDP, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // fill server information
    servaddr.sin_family = IPV4;      // protocol
    servaddr.sin_port = htons(PORT);     // port number
    servaddr.sin_addr.s_addr = INADDR_ANY; // Ip addr

    int n, len;

    sendto(sockfd, (const char *)msg, strlen(msg), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Hello message sent.\n");
            
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}