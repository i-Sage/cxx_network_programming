#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>



int main() {
    struct sockaddr_in from_addr = {0};
    socklen_t from_addr_len = sizeof(struct sockaddr_in);
    char udp_server_buf[64 + 1] = {0};
    const char *udp_server_send_buf = "broadcast server";

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Set up server address to listen for broadcasts
    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = htons(60000),
        .sin_addr.s_addr = htonl(INADDR_ANY),  // Use INADDR_ANY instead of INADDR_BROADCAST
    };

    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));

    // Receive broadcast data
    for (;;) {
        memset(udp_server_buf, 0, sizeof(udp_server_buf));  // Clear buffer before receiving

        recvfrom(sock, udp_server_buf, sizeof(udp_server_buf) - 1, 0,
                 (struct sockaddr *)&from_addr, &from_addr_len);  // Store sender's address

        std::cout << "Received UDP from "
                  << inet_ntoa(from_addr.sin_addr) << "\t"
                  << ntohs(from_addr.sin_port)
                  << " [DATA] " << udp_server_buf << "\n";

        if (strcmp(udp_server_buf, "Are you Espressif ?") == 0) {
            std::cout << "Pass Phrase received\n";
            sendto(sock, udp_server_send_buf, strlen(udp_server_send_buf), 0,
                   (struct sockaddr *)&from_addr, from_addr_len);
        }
    }

    close(sock);
}
