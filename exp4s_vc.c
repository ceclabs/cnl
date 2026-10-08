#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define SIZE 1024

int main()
{
    int sockfd;
    char str[SIZE];
    int vowels = 0, consonants = 0;

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

    for (int i = 0; str[i] != '\0'; i++)
    {
        char ch = tolower(str[i]);

        if (ch == 'a' || ch == 'e' || ch == 'i' ||
            ch == 'o' || ch == 'u')
        {
            vowels++;
        }
        else if (isalpha(ch))
        {
            consonants++;
        }
    }

    sendto(sockfd, &vowels, sizeof(vowels), 0,
           (struct sockaddr *)&client, client_len);

    sendto(sockfd, &consonants, sizeof(consonants), 0,
           (struct sockaddr *)&client, client_len);

    close(sockfd);

    return 0;
}
