#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002

int main()
{
    int sockfd;
    double num1, num2, result;
    char operator;

    struct sockaddr_in server;
    socklen_t server_len = sizeof(server);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter first number: ");
    scanf("%lf", &num1);

    printf("Enter operator (+ - * /): ");
    scanf(" %c", &operator);

    printf("Enter second number: ");
    scanf("%lf", &num2);

    sendto(sockfd, &num1, sizeof(num1), 0,
           (struct sockaddr *)&server, server_len);

    sendto(sockfd, &operator, sizeof(operator), 0,
           (struct sockaddr *)&server, server_len);

    sendto(sockfd, &num2, sizeof(num2), 0,
           (struct sockaddr *)&server, server_len);

    recvfrom(sockfd, &result, sizeof(result), 0,
             (struct sockaddr *)&server, &server_len);

    printf("\nResult = %.2lf\n", result);

    close(sockfd);

    return 0;
}
