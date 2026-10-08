#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void send_all(int sock, void *buf, int len)
{
    int sent = 0;

    while (sent < len)
    {
        int n = send(sock, (char *)buf + sent, len - sent, 0);

        if (n <= 0)
        {
            perror("send failed");
            exit(1);
        }

        sent += n;
    }
}

void recv_all(int sock, void *buf, int len)
{
    int received = 0;

    while (received < len)
    {
        int n = recv(sock, (char *)buf + received,
                     len - received, 0);

        if (n <= 0)
        {
            perror("recv failed");
            exit(1);
        }

        received += n;
    }
}

int main()
{
    int client_socket;

    double num1, num2, result;
    char operator;
    char choice;

    /* Create socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* Server address */
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9090);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect */
    if (connect(client_socket,
                (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0)
    {
        perror("Connect failed");
        exit(1);
    }

    printf("Connected to server!\n");

    do
    {
        /* Input */
        printf("\nEnter first number: ");
        scanf("%lf", &num1);

        printf("Enter operator (+, -, *, /): ");
        scanf(" %c", &operator);

        printf("Enter second number: ");
        scanf("%lf", &num2);

        /* Send data */
        send_all(client_socket, &num1, sizeof(num1));
        send_all(client_socket, &operator, sizeof(operator));
        send_all(client_socket, &num2, sizeof(num2));

        /* Receive result */
        recv_all(client_socket, &result, sizeof(result));

        /* Display result */
        printf("\nResult = %.2lf\n", result);

        /* Continue */
        printf("\nDo you want to continue? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    /* Send special operator to tell server to stop */
    operator = 'q';

    send_all(client_socket, &operator, sizeof(operator));

    close(client_socket);

    return 0;
}
