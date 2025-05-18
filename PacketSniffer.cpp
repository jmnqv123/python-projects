#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

// IP header structure (simplified)
struct IPHeader {
    unsigned char  iph_ihl : 4;
    unsigned char  iph_ver : 4;
    unsigned char  iph_tos;
    unsigned short iph_len;
    unsigned short iph_ident;
    unsigned short iph_flags_offset;
    unsigned char  iph_ttl;
    unsigned char  iph_protocol;
    unsigned short iph_checksum;
    unsigned int   iph_sourceip;
    unsigned int   iph_destip;
};

// Convert IP address from unsigned int to dotted decimal string
std::string ipToString(unsigned int ip) {
    struct in_addr ip_addr;
    ip_addr.S_un.S_addr = ip;
    return std::string(inet_ntoa(ip_addr));
}

int main() {
    WSADATA wsaData;
    SOCKET snifferSocket;
    sockaddr_in source, dest;

    char buffer[65536];
    int buffer_len = sizeof(buffer);

    std::cout << "Starting Packet Sniffer..." << std::endl;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Create a raw socket that will sniff IP packets
    snifferSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (snifferSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create raw socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Get local IP address of the first adapter (simple way)
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        std::cerr << "Failed to get hostname." << std::endl;
        closesocket(snifferSocket);
        WSACleanup();
        return 1;
    }
    hostent* localHost = gethostbyname(hostname);
    if (!localHost) {
        std::cerr << "Failed to get host info." << std::endl;
        closesocket(snifferSocket);
        WSACleanup();
        return 1;
    }

    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr = *((in_addr*)localHost->h_addr);

    // Bind the socket to the local IP address
    if (bind(snifferSocket, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(snifferSocket);
        WSACleanup();
        return 1;
    }

    // Set socket options to include IP headers
    int optval = 1;
    if (setsockopt(snifferSocket, IPPROTO_IP, IP_HDRINCL, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        std::cerr << "Failed to set socket options. Error: " << WSAGetLastError() << std::endl;
        closesocket(snifferSocket);
        WSACleanup();
        return 1;
    }

    // Enable promiscuous mode (packet sniffing)
    if (ioctlsocket(snifferSocket, SIO_RCVALL, (u_long*)&optval) == SOCKET_ERROR) {
        std::cerr << "Failed to enable promiscuous mode. Error: " << WSAGetLastError() << std::endl;
        closesocket(snifferSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Sniffing... Press Ctrl+C to stop." << std::endl;

    while (true) {
        int received_len = recv(snifferSocket, buffer, buffer_len, 0);
        if (received_len == SOCKET_ERROR) {
            std::cerr << "recv failed. Error: " << WSAGetLastError() << std::endl;
            break;
        }

        // Parse IP header
        IPHeader* ipHeader = (IPHeader*)buffer;
        std::string sourceIP = ipToString(ipHeader->iph_sourceip);
        std::string destIP = ipToString(ipHeader->iph_destip);
        int protocol = ipHeader->iph_protocol;

        std::cout << "Packet captured: ";
        std::cout << "From: " << sourceIP << " To: " << destIP << " Protocol: " << protocol << std::endl;
    }

    // Cleanup
    optval = 0;
    ioctlsocket(snifferSocket, SIO_RCVALL, (u_long*)&optval); // Disable promiscuous mode
    closesocket(snifferSocket);
    WSACleanup();

    return 0;
}
