#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void handle_client(SOCKET client_socket) {
    char buffer[3000];
    int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRead] = '\0';

    std::cout << "📥 Request:\n" << buffer << std::endl;

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body><h1>Hello from C++ Server 🖥️</h1></body></html>";

    send(client_socket, response.c_str(), response.size(), 0);
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, SOMAXCONN);

    std::cout << "🚀 Server running at http://localhost:" << PORT << std::endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        std::thread t(handle_client, client_socket);
        t.detach();
    }

    WSACleanup();
    return 0;
}

