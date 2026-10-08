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
    int server_socket;
    int client_socket;

    double num1, num2, result;
    char operator;

    /* Create socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* Server address */
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9090);
    server_address.sin_addr.s_addr = INADDR_ANY;

    /* Bind */
    if (bind(server_socket,
             (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    /* Listen */
    listen(server_socket, 5);

    printf("Server waiting for client......\n");

    /* Accept */
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket < 0)
    {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected!\n");

    while (1)
    {
        /* Receive first number */
        recv_all(client_socket, &num1, sizeof(num1));

        /* Receive operator */
        recv_all(client_socket, &operator, sizeof(operator));

        /* Check exit */
        if (operator == 'q')
            break;

        /* Receive second number */
        recv_all(client_socket, &num2, sizeof(num2));

        printf("\nReceived: %.2lf %c %.2lf\n",
               num1, operator, num2);

        /* Perform calculation */
        switch (operator)
        {
            case '+':
                result = num1 + num2;
                break;

            case '-':
                result = num1 - num2;
                break;

            case '*':
                result = num1 * num2;
                break;

            case '/':
                if (num2 == 0)
                {
                    printf("Error: Division by zero\n");
                    result = 0;
                }
                else
                {
                    result = num1 / num2;
                }
                break;

            default:
                printf("Invalid operator\n");
                result = 0;
        }

        printf("Result = %.2lf\n", result);

        /* Send result */
        send_all(client_socket, &result, sizeof(result));
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
