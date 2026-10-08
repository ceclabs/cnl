#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define SIZE 50

int main()
{
    int sockfd;
    int n;
    int result[SIZE];

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter number of terms: ");
    scanf("%d", &n);

    sendto(sockfd, &n, sizeof(n), 0,
           (struct sockaddr *)&server, server_len);

    recvfrom(sockfd, result, sizeof(result), 0,
             (struct sockaddr *)&server, &server_len);

    printf("\nFibonacci Series:\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", result[i]);
    }

    printf("\n");

    close(sockfd);

    return 0;
}
