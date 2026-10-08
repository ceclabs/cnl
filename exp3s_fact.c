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

    int n;
    unsigned long long result;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9090);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket,
             (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    listen(server_socket, 5);

    printf("Server waiting for client......\n");

    client_socket = accept(server_socket, NULL, NULL);

    printf("Client connected!\n");

    while (1)
    {
        recv_all(client_socket, &n, sizeof(n));

        if (n == 0)
            break;

        result = 1;

        for (int i = 1; i <= n; i++)
        {
            result = result * i;
        }

        send_all(client_socket, &result, sizeof(result));
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
