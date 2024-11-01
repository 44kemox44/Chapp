#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
        // Handle error
    }


    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Chat loop
    while (true) {
        std::cout << "You: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(sock, buffer, strlen(buffer), 0);

        int bytes_read = recv(sock, buffer, BUFFER_SIZE, 0);
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
