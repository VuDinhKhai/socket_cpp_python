//#include <iostream>
//#include <string>
//#include <winsock2.h>
//#include <Ws2tcpip.h>
//
//#pragma comment(lib, "Ws2_32.lib")
//
//int main() {
//    int port = 8080;
//    std::string server_ip = "127.0.0.1";
//
//    // Initialize Winsock
//    WSADATA wsaData;
//    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (result != 0) {
//        std::cerr << "WSAStartup failed: " << result << "\n";
//        return 1;
//    }
//
//    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (server_socket == INVALID_SOCKET) {
//        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
//        WSACleanup();
//        return 1;
//    }
//
//    sockaddr_in server_address;
//    server_address.sin_family = AF_INET;
//    server_address.sin_port = htons(port);
//
//    // Convert IPv4 and IPv6 addresses from text to binary form
//    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) != 1) {
//        std::cerr << "Invalid address/Address not supported: " << server_ip << "\n";
//        closesocket(server_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
//        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
//        closesocket(server_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
//        std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
//        closesocket(server_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    std::cout << "Starting to listen\n";
//
//    SOCKET client_socket;
//    sockaddr_in client_address;
//    int client_address_length = sizeof(client_address);
//    client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_length);
//    if (client_socket == INVALID_SOCKET) {
//        std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
//        closesocket(server_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    char client_ip[INET_ADDRSTRLEN];
//    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
//    std::cout << "Connected by " << client_ip << ":" << ntohs(client_address.sin_port) << "\n";
//
//    while (true) {
//        char buffer[1000];
//        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
//        if (bytes_received == SOCKET_ERROR) {
//            std::cerr << "Receive failed with error: " << WSAGetLastError() << "\n";
//            closesocket(client_socket);
//            closesocket(server_socket);
//            WSACleanup();
//            return 1;
//        }
//        if (bytes_received == 0) {
//            std::cout << "Connection closed by client\n";
//            break;
//        }
//        buffer[bytes_received] = '\0'; // Null-terminate the received data to treat it as a string
//        std::cout << "Received text: " << buffer << "\n";
//    }
//
//    closesocket(client_socket);
//    closesocket(server_socket);
//    WSACleanup();
//
//    return 0;
//}

#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    int port = 8080;

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    // Create socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    // Bind socket
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << port << "...\n";

    // Accept incoming connection
    SOCKET client_socket;
    sockaddr_in client_address;
    int client_address_length = sizeof(client_address);
    client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_length);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected\n";

    // Main loop for chat
    char buffer[1000];
    while (true) {
        // Receive message from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Receive failed with error: " << WSAGetLastError() << "\n";
            break;
        }
        if (bytes_received == 0) {
            std::cout << "Client disconnected\n";
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        // Send message to client
        std::string reply;
        std::cout << "Server: ";
        std::getline(std::cin, reply);
        int bytes_sent = send(client_socket, reply.c_str(), reply.length(), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Send failed with error: " << WSAGetLastError() << "\n";
            break;
        }
    }

    // Close sockets and cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
