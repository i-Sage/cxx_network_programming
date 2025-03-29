/// The setup function takes a host and service, specified in a string,
/// and returns a connected socket (or -1 on failure). The host or
/// service may be specified as NULL


#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void DieWithUserMessage(const char *msg, const char *detail) {
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}


int SetupTCPClientSocket(const char *host, const char *service) {
    /// Tell the system what kind(s) of address info we want
    struct addrinfo addr_criteria;                 // Criteria for address match
    memset(&addr_criteria, 0, sizeof(addrinfo));   // Zero out struture
    addr_criteria.ai_family = AF_UNSPEC;           // v4 or v6 is OK
    addr_criteria.ai_socktype = SOCK_STREAM;       // Only streaming sockets
    addr_criteria.ai_protocol = IPPROTO_TCP;       // Only TCP protocol
    
    // Get address(es)
    struct addrinfo *serv_addr;                   // Holder for returned list of server addresses
    
    int rtn_value = getaddrinfo(host, service, &addr_criteria, &serv_addr);
    if (rtn_value != 0) {
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtn_value));
    }
    
    int sock = -1;
    for (auto addr = serv_addr; addr != nullptr; addr = addr->ai_next) {
        // Create a reliable, stream socket using TCP
        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock < 0) { continue; } // Socket creation failed; try next address

        // Establish the connection to the echo server
        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
            break;    // Success !!, break and return this socket
        }

        close(sock);  // Socket connection failed; try next address
        sock = -1; 
    }
    freeaddrinfo(serv_addr);  // Free addrinfo allocated in getaddrinfo()
    return sock;
}