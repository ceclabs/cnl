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
        int n = recv(sock, (char *)buf + received, len - received, 0);

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
    int server_socket, client_socket;
    char str[100];
    char reversed[100];
    int length;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9090);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(server_socket,
             (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen
    listen(server_socket, 5);

    printf("Server waiting for client......\n");

    // Accept
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket < 0)
    {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected!\n");

    while (1)
    {
        // Receive string length
        recv_all(client_socket, &length, sizeof(length));

        if (length == 0)
            break;

        // Receive string
        recv_all(client_socket, str, length);

        printf("\nReceived string: %s\n", str);

        // Reverse the string
        for (int i = 0; i < length - 1; i++)
        {
            reversed[i] = str[length - 2 - i];
        }

        reversed[length - 1] = '\0';

        printf("Reversed string: %s\n", reversed);

        // Send reversed string
        send_all(client_socket, reversed, length);
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
