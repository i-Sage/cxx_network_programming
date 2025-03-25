
#include "utils.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t MAXPENDING = 5; // Maximum outstanding connection requests
constexpr size_t BUFSIZE {1024};

void HandleTCPClient(int clnt_socket) {
    char buffer[BUFSIZE]; // Buffer for echo string
    
    // Send message to client
    std::string pass_phrase = "hello wor";
    send(clnt_socket, pass_phrase.c_str(), pass_phrase.size(), 0);
    
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


int main(int argc, char *argv[]) {
  if (argc != 2) {
    DieWithUserMessage("Parameter(s)", "<Server Port>");
  }

  in_port_t serv_port = atoi(argv[1]);

  // create socket for incoming connections
  int serv_sock;
  if ((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithSystemMessage("socket() failed");
  }

  // construct local address structure
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(serv_port);

  // Bind to the local address
  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    DieWithSystemMessage("bind() failed");
  }

  // Mark the socket so it will listen for incoming connections
  if (listen(serv_sock, MAXPENDING) < 0) {
    DieWithSystemMessage("listen() failed");
  }

  for (;;) {                      // Run forever
    struct sockaddr_in clnt_addr; // client address
    // set length of client address structure (in-out parameter)
    socklen_t clnt_addr_len = sizeof(clnt_addr);

    // Wait for a client to connect
    int clnt_sock =
        accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    if (clnt_sock < 0) {
      DieWithSystemMessage("accept() failed");
    }

    // clnt sock is connected to a client !
    char clnt_name[INET_ADDRSTRLEN]; // string to contain client address
    if (inet_ntop(AF_INET, &clnt_addr.sin_addr.s_addr, clnt_name,
                  sizeof(clnt_name)) != nullptr) {
      printf("Handling client %s/%d\n", clnt_name, ntohs(clnt_addr.sin_port));
    } else {
      puts("unable to get client address");
    }
    HandleTCPClient(clnt_sock);
  }
}