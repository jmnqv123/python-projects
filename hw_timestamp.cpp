#include <winsock2.h>    // Include this first
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstdint>       // For uint64_t
#include <cstring>       // For strlen

#pragma comment(lib, "ws2_32.lib")

LARGE_INTEGER frequency;

inline uint64_t get_highres_time_ns() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<uint64_t>((now.QuadPart * 1e9) / frequency.QuadPart);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // Initialize high resolution timer
    QueryPerformanceFrequency(&frequency);

    for (int i = 0; i < 5; ++i) {
        const char* msg = "timestamp test";
        uint64_t ts = get_highres_time_ns();

        sendto(sock, msg, strlen(msg), 0, (sockaddr*)&addr, sizeof(addr));

        std::cout << "Sent message with timestamp: " << ts << " ns\n";
        Sleep(1000);  // simulate periodic sending
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
