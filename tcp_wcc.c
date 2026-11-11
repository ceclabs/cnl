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
    int client_socket;
    char str[SIZE] = {0};
    int result;

    // Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect failed");
        exit(1);
    }
    printf("Connected to server!\n");

    // Input and send
    printf("Enter a sentence: ");
    fgets(str, SIZE, stdin);
    str[strcspn(str, "\n")] = '\0';   // remove newline
    send_all(client_socket, str, SIZE);

    // Receive and display result
    recv_all(client_socket, &result, sizeof(result));
    printf("\nNumber of words: %d\n", result);

    close(client_socket);

    return 0;
}
