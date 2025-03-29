#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int opt_val = 1;
    struct sockaddr_in from_addr = {0};
    socklen_t from_addr_len = sizeof(struct sockaddr_in);
    char udp_recv_buf[64 + 1] = {0};

    // Create a UDP broadcast socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Enable broadcasting
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt_val, sizeof(opt_val));

    // Set broadcast destination address and port
    struct sockaddr_in destination_address = {
        .sin_family = AF_INET,
        .sin_port   = htons(60000),
        .sin_addr.s_addr = htonl(INADDR_BROADCAST),
    };

    // Broadcast message
    const char *broad_cast_msg = "Are you Espressif ?";

    // Send broadcast message
    sendto(sock, broad_cast_msg, strlen(broad_cast_msg), 0,
           (struct sockaddr *)&destination_address, sizeof(destination_address));

    // Receive response
    std::cout << "Waiting for response...\n";
    recvfrom(sock, udp_recv_buf, sizeof(udp_recv_buf) - 1, 0,
             (struct sockaddr *)&from_addr, &from_addr_len);

    // Print received response
    std::cout << "Received UDP unicast from "
              << inet_ntoa(from_addr.sin_addr) << "\t"
              << ntohs(from_addr.sin_port)
              << " [DATA] " << udp_recv_buf << "\n";

    close(sock);
}