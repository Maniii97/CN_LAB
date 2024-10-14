#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define IPV4 AF_INET
#define TCP SOCK_STREAM

void reverse(char* s){
    int n = strlen(s);
    for(int i=0;i<n/2;i++){
        char temp = s[i];
        s[i] = s[n-i-1];
        s[n-i-1] = temp;
    }
    int l = 0;
    for(int i=0;i<=n;i++){
        if(s[i] == ' ' || s[i] == '\0'){
            int r = i-1;
            while(l < r){
                char temp = s[l];
                s[l] = s[r];
                s[r] = temp;
                l++;
                r--;
            }
            l = i+1;
        }
    }
}

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in serv, cli;
    sockfd = socket(IPV4, TCP, 0);

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) != 0) {
        printf("Error in bind");
        exit(0);
    }

    if (listen(sockfd, 5) != 0) {
        printf("Listen Error");
        exit(0);
    }

    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        printf("Failed accept");
        exit(0);
    }

    char buffer[1000];
    bzero(buffer, sizeof(buffer));
    int n = read(connfd, buffer, sizeof(buffer));
    buffer[n] = '\0';
    char* clientIp = inet_ntoa(cli.sin_addr);
    int clientPort = ntohs(cli.sin_port);
    char buff[n+1];
    for(int i=0;i<=n;i++){
        buff[i] = buffer[i];
    }
    reverse(buff);
    printf("Client : %s\n", buff);
    printf("Client IP: %s\n", clientIp);
    printf("Client Port: %d\n", clientPort);

    char *res = "Hi from server";
    write(connfd, res, strlen(res));

    close(connfd);
    close(sockfd);
}
