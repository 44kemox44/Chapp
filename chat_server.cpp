// chat_server.cpp
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return -1;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return -1;
    }

    // Set up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Waiting for a connection..." << std::endl;

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected!" << std::endl;

    // Chat loop
    while (true) {
        std::cout << "Server: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(new_socket, buffer, strlen(buffer), 0);

        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_read == SOCKET_ERROR) {
            std::cerr << "Error receiving message" << std::endl;
            break;
        }
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        std::cout << "Client: " << buffer << std::endl;
    }

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
