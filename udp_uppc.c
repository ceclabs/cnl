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

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Input and send
    printf("Enter a string: ");
    fgets(str, SIZE, stdin);
    str[strcspn(str, "\n")] = '\0';   // remove newline
    sendto(sockfd, str, strlen(str) + 1, 0,
           (struct sockaddr *)&server, server_len);

    // Receive and display result
    recvfrom(sockfd, result, SIZE, 0,
             (struct sockaddr *)&server, &server_len);
    printf("\nUppercase string: %s\n", result);

    close(sockfd);

    return 0;
}
