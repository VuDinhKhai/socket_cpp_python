//#include <iostream>
//#include <string>
//#include <winsock2.h>
//#include <Ws2tcpip.h> // Include this for inet_pton
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
//    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (client_socket == INVALID_SOCKET) {
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
//    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
//        std::cerr << "Invalid address/Address not supported: " << server_ip << "\n";
//        closesocket(client_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
//        std::cerr << "Error connecting to server: " << WSAGetLastError() << "\n";
//        closesocket(client_socket);
//        WSACleanup();
//        return 1;
//    }
//
//    std::cout << "Connected to server\n";
//
//    while (true) {
//        std::string text;
//        std::cout << "Enter a text string: ";
//        std::getline(std::cin >> std::ws, text); // Read the input text including spaces
//
//        // Send the text string to the server
//        int bytes_sent = send(client_socket, text.c_str(), text.size(), 0);
//        if (bytes_sent == SOCKET_ERROR) {
//            std::cerr << "Error sending data to server: " << WSAGetLastError() << "\n";
//            break;
//        }
//    }
//
//    closesocket(client_socket);
//    WSACleanup();
//
//    return 0;
//}

#include <iostream>
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h> // Include this for inet_pton

#pragma comment(lib, "Ws2_32.lib")

int main() {
    int port = 8080;
    std::string server_ip = "127.0.0.1";

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported: " << server_ip << "\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << "\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server\n";

    // Chat loop
    while (true) {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        // Send message to server
        int bytes_sent = send(client_socket, message.c_str(), message.size(), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Error sending data to server: " << WSAGetLastError() << "\n";
            break;
        }

        // Receive response from server
        char buffer[1000];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Error receiving data from server: " << WSAGetLastError() << "\n";
            break;
        }

        buffer[bytes_received] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
