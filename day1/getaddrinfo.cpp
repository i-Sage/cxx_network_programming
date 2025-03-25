/// The program getaddrinfo.c takes two command-line parameters, a hostname(or address)
/// and a serive name (or port number), and prints the address information returned by
/// getaddrinfo().

#include "diewithmessage.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { // Check if correct number of arguments passed
        DieWithUserMessage("Parameters(s)", "<Address/Name> <Port/Service>");
    }
    
    char *addr_string = argv[1];   // Server address/name
    char *port_string = argv[2];   // Server port/service
    
    // Create a filter for what kind(s) of address info we want
    struct addrinfo address_criteria;
    memset(&address_criteria, 0, sizeof(addrinfo));
    address_criteria.ai_family = AF_UNSPEC;                  // Any address family
    address_criteria.ai_socktype = SOCK_STREAM;              // Only stream sockets
    address_criteria.ai_protocol = IPPROTO_TCP;              // Only TCP protocol
    
    // Get address(es) associated with the specified name/service
    struct addrinfo *addr_list;                              // Holder for list of addresses returned
    // Modify serv_addr contents to reference linked list of addresses
    int rtn_value = getaddrinfo(addr_string, port_string, &address_criteria, &addr_list);
    if (rtn_value != 0) {
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtn_value));
    }
    
    // Display returned addresses
    for (auto addr = addr_list; addr != nullptr; addr = addr->ai_next) {
        PrintSocketAddress(addr->ai_addr, stdout);
        fputc('\n', stdout);
    }
    
    freeaddrinfo(addr_list);
    exit(0); 
}