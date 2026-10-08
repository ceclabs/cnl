#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002

int main()
{
    int sockfd;
    int n;
    unsigned long long result;

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter a number: ");
    scanf("%d", &n);

    sendto(sockfd, &n, sizeof(n), 0,
           (struct sockaddr *)&server, server_len);

    recvfrom(sockfd, &result, sizeof(result), 0,
             (struct sockaddr *)&server, &server_len);

    printf("\nFactorial = %llu\n", result);

    close(sockfd);

    return 0;
}
