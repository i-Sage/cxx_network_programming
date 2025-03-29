#include <asm-generic/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>


int main(int argc, char *argv[]) {
    // Our Socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  

    // Lets retrieve some socket information like the size of the buffer
    unsigned recv_buffer_size;
    socklen_t sock_opt_size = sizeof(recv_buffer_size);
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_buffer_size, &sock_opt_size);
    
    // Display the information
    printf("RECV BUFFER SIZE: %u\n", recv_buffer_size);

    // Lets double the size
    recv_buffer_size *= 2;
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_buffer_size, sizeof(recv_buffer_size));

    // Lets display this info again
    recv_buffer_size = 0;
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_buffer_size, &sock_opt_size);
    printf("RECV BUFFER SIZE: %u\n", recv_buffer_size);

    /// ! The value passed to setsockopt() is not guaranteed to be the new size of the socket
    /// ! buffer, even if the call succeds. Rather, it is best thought of as a "hint" to the
    /// ! system about the value desired by the user, the system, after all has to manage all
    /// ! resoures for all user and may consider other factors in adjusting buffer size

}