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

    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("UDP Server waiting...\n");

    recvfrom(sockfd, &n, sizeof(n), 0,
             (struct sockaddr *)&client, &client_len);

    printf("Received n = %d\n", n);

    int a = 0;
    int b = 1;

    for (int i = 0; i < n; i++)
    {
        result[i] = a;

        int next = a + b;
        a = b;
        b = next;
    }

    sendto(sockfd, result, sizeof(result), 0,
           (struct sockaddr *)&client, client_len);

    close(sockfd);

    return 0;
}
