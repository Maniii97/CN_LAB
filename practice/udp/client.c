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

int main(){
    struct sockaddr_in serv, cli;
    char buffer[MAXLINE];
    char *msg = "2 3 4 6 5";
    int sockfd = socket(IPV4, UDP, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    bzero(&serv, sizeof(serv));
    bzero(&cli, sizeof(cli));

    serv.sin_family = IPV4;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *)&serv, sizeof(serv));
    printf("Hello message sent.\n");
            
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serv, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}