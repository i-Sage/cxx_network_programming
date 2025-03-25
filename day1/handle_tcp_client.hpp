#pragma once

#include "diewithmessage.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>

void HandleTCPClient(int clnt_socket) {
    char buffer[BUFSIZE]; // Buffer for echo string
    
    // Reveive message from client
    ssize_t num_bytes_rcvd = recv(clnt_socket, buffer, BUFSIZE, 0);
    if (num_bytes_rcvd < 0) {
        DieWithSystemMessage("recv() failed");
    }
    
    // Send received string and receive again until end of stream
    while (num_bytes_rcvd > 0) {
        // Echo message back to client
        ssize_t num_bytes_send = send(clnt_socket, buffer, num_bytes_rcvd, 0);
        if (num_bytes_send < 0) {
            DieWithSystemMessage("send() failed");
        } else if (num_bytes_send != num_bytes_rcvd) {
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        }
        
        // See if there is more data to receive
        num_bytes_rcvd = recv(clnt_socket, buffer, BUFSIZE, 0);
        if (num_bytes_rcvd < 0) {
            DieWithUserMessage("recv()", "failed");
        }
    }
    close(clnt_socket);
}