#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define SIZE 1024

int main()
{
    int sockfd;
    char str[SIZE];
    int vowels, consonants;

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

    recvfrom(sockfd, &vowels, sizeof(vowels), 0,
             (struct sockaddr *)&server, &server_len);

    recvfrom(sockfd, &consonants, sizeof(consonants), 0,
             (struct sockaddr *)&server, &server_len);

    printf("\nVowels = %d\n", vowels);
    printf("Consonants = %d\n", consonants);

    close(sockfd);

    return 0;
}
