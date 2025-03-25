/// The typical TCP client's communication involves four basic steps:
/// 1. Create a TCP socket using socket()
/// 2. Establish a connection to the server using connect()
/// 3. Communicate using send and recv()
/// 4. Close the connetion with close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "diewithmessage.hpp"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        DieWithUserMessage("Parameter(s)",
            "<Server Address> <Echo Word> [<Server Port>]");
    }
    char *server_ip = argv[1];   // Server IP address
    char *echo_string = argv[2]; // Second arg: string to echo
    
    // Third arg (optional): server port (numeric).
    in_port_t server_port = atoi(argv[3]); 
    
    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        DieWithSystemMessage("socket() failed");
    }
    
    // Construct the server address structure
    struct sockaddr_in serv_addr;              // Server address
    memset(&serv_addr, 0, sizeof(serv_addr));  // Zero out structure
    serv_addr.sin_family = AF_INET;            // IPv4 address family
    
    // Convert Address
    int rtn_value = inet_pton(AF_INET, server_ip, &serv_addr.sin_addr.s_addr);
    if (rtn_value == 0) {
        DieWithSystemMessage("inet_pton() failed");
    } else if (rtn_value < 0) {        
        DieWithSystemMessage("inet_pton() failed");
    }
    serv_addr.sin_port = htons(server_port);  // Server port
    
    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        DieWithSystemMessage("connect() failed");
    }
    
    size_t echo_string_len = strlen(echo_string); // Determine input length
    
    // send the string to the server
    ssize_t num_bytes = send(sock, echo_string, echo_string_len, 0);
    if (num_bytes < 0) {        
        DieWithSystemMessage("send() failed");
    } else if (num_bytes != echo_string_len) {
        DieWithUserMessage("send() ", "send unexpected number of bytes"); 
    }
    
    // Receive the same string back from the server
    unsigned int total_bytes_rcvd = 0;   // count the total bytes received
    fputs("Reveived: ", stdout);         // Setup to print the echoed string
    while (total_bytes_rcvd < echo_string_len) {
        char buffer[BUFSIZE];
        num_bytes = recv(sock, buffer, BUFSIZE - 1, 0);
        if (num_bytes < 0) {
            DieWithSystemMessage("recv() failed");
        } else if (num_bytes == 0) {
            DieWithUserMessage("recv() ", "connection closed permaturely"); 
        }
        total_bytes_rcvd += num_bytes;   // Keep tally of total bytes
        buffer[num_bytes] = '\0';        // terminate the string!
        fputs(buffer, stdout);           // Print the echo buffer
    }
    fputc('\n', stdout);
    close(sock);
    exit(0);
}
