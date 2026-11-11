#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9002
#define SIZE 1024

int main()
{
    int sockfd;
    char str[SIZE] = {0};
    char result[SIZE] = {0};

    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed");
        exit(1);
    }

    printf("UDP Server waiting...\n");

    // Receive data from client
    recvfrom(sockfd, str, SIZE, 0,
             (struct sockaddr *)&client, &client_len);
    printf("Received: %s\n", str);

    // Process
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        result[i] = str[len - 1 - i];
    result[len] = '\0';

    // Send result back
    sendto(sockfd, result, strlen(result) + 1, 0,
           (struct sockaddr *)&client, client_len);

    close(sockfd);

    return 0;
}
