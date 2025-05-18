#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);  // Initialize Winsock

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Port
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 1);
    std::cout << "Server is running. Waiting for connection...\n";

    SOCKET client;
    client = accept(server, NULL, NULL);
    std::cout << "Client connected!\n";

    char buffer[1024];
    while (true) {
        int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        std::cout << "Client: " << buffer;

        std::string reply;
        std::cout << "You: ";
        std::getline(std::cin, reply);
        send(client, reply.c_str(), reply.length(), 0);
    }

    closesocket(client);
    closesocket(server);
    WSACleanup();
    return 0;
}
