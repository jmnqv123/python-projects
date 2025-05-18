#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "Connected to server!\n";

    char buffer[1024];
    while (true) {
        std::string msg;
        std::cout << "You: ";
        std::getline(std::cin, msg);
        send(sock, msg.c_str(), msg.length(), 0);

        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        std::cout << "Server: " << buffer << "\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

