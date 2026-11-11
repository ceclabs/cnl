#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9090
#define SIZE 1024

void send_all(int sock, void *buf, int len) {
    int sent = 0;
    while (sent < len) {
        int n = send(sock, (char *)buf + sent, len - sent, 0);
        if (n <= 0) { perror("send failed"); exit(1); }
        sent += n;
    }
}

void recv_all(int sock, void *buf, int len) {
    int received = 0;
    while (received < len) {
        int n = recv(sock, (char *)buf + received, len - received, 0);
        if (n <= 0) { perror("recv failed"); exit(1); }
        received += n;
    }
}

int main()
{
    int server_socket, client_socket;
    char str[SIZE] = {0};
    int result;

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Allow quick restart of the server on the same port
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind, listen, accept
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(1);
    }

    listen(server_socket, 5);
    printf("Server waiting for client......\n");

    client_socket = accept(server_socket, NULL, NULL);
    printf("Client connected!\n");

    // Receive data from client
    recv_all(client_socket, str, SIZE);
    printf("Received: %s\n", str);

    // Process
    result = 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (strchr("aeiouAEIOU", str[i]) != NULL)
            result++;

    // Send result back
    send_all(client_socket, &result, sizeof(result));

    close(client_socket);
    close(server_socket);

    return 0;
}
