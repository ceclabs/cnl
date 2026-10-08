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
    char reversed[SIZE];

    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("UDP Server waiting...\n");

    recvfrom(sockfd, str, SIZE, 0,
             (struct sockaddr *)&client, &client_len);

    printf("Received: %s\n", str);

    int length = strlen(str);

    for (int i = 0; i < length; i++)
    {
        reversed[i] = str[length - 1 - i];
    }

    reversed[length] = '\0';

    printf("Reversed: %s\n", reversed);

    sendto(sockfd, reversed, strlen(reversed) + 1, 0,
           (struct sockaddr *)&client, client_len);

    close(sockfd);

    return 0;
}
