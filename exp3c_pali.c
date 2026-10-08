#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    char str[100];
    char result[20];
    char choice;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9090);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

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
        printf("\nEnter a string: ");
        scanf("%99s", str);

        int length = strlen(str) + 1;

        send_all(client_socket, &length, sizeof(length));
        send_all(client_socket, str, length);

        recv_all(client_socket, result, sizeof(result));

        printf("\nResult: %s\n", result);

        printf("\nDo you want to continue? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    int length = 0;
    send_all(client_socket, &length, sizeof(length));

    close(client_socket);

    return 0;
}
