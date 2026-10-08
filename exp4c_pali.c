#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define SIZE 100

int main()
{
    int sockfd;
    char str[SIZE];
    char result[30];

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter a string: ");
    fgets(str, SIZE, stdin);

    str[strcspn(str, "\n")] = '\0';

    sendto(sockfd, str, strlen(str) + 1, 0,
           (struct sockaddr *)&server, server_len);

    recvfrom(sockfd, result, SIZE, 0,
             (struct sockaddr *)&server, &server_len);

    printf("\n%s\n", result);

    close(sockfd);

    return 0;
}
