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

int sum(char buffer[]){
    int sum = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(buffer[i] == ' '){
            continue;
        }
        sum += buffer[i] - '0';
    }
    return sum;
}

int avg(char buffer[]){
    int avg = sum(buffer) / 5;
}

int median(char buffer[]){
    int arr[5];
    int n = strlen(buffer);
    int idx = 0;
    for(int i = 0; i <= n; i++){
        if(buffer[i] == ' ' || buffer[i] == '\0'){
            continue;
        }
        arr[idx++] = buffer[i] - '0';
    }
    for(int i = 0; i < 5; i++){
        for(int j = i + 1; j < 5; j++){
            if(arr[i] > arr[j]){
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr[2];
}

int main(){
    struct sockaddr_in serv, cli;
    char buffer[MAXLINE];
    char *msg = "Hello from server";
    int sockfd = socket(IPV4, UDP, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&serv, 0, sizeof(serv));
    memset(&cli, 0, sizeof(cli));

    serv.sin_family = IPV4;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *)&serv, sizeof(serv)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    int len , n; // len is value/result, n is the number of bytes received
    len = sizeof(cli);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cli, &len);

    buffer[n] = '\0';

    printf("Client : %s\n", buffer);
    int sm = sum(buffer);
    int av = avg(buffer);
    int mi = median(buffer);
    printf("Sum = %d , Avg = %d , Median = %d\n",sm,av,mi);

    sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&cli, len);
    printf("Hello message sent.\n");

    close(sockfd);

    return 0;
}