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

    int left = 0;
    int right = strlen(str) - 1;
    int palindrome = 1;

    while (left < right)
    {
        if (str[left] != str[right])
        {
            palindrome = 0;
            break;
        }

        left++;
        right--;
    }

    if (palindrome)
        sprintf(result, "Palindrome");
    else
        sprintf(result, "Not Palindrome");

    sendto(sockfd, result, strlen(result) + 1, 0,
           (struct sockaddr *)&client, client_len);

    close(sockfd);

    return 0;
}
