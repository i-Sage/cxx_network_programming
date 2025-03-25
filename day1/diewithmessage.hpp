#pragma once

#include <cstddef>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>

constexpr size_t BUFSIZE = 1024;

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

void PrintSocketAddress(const struct sockaddr *address, FILE *stream) {
  // Test for address and stream
  if (address == nullptr || stream == nullptr) {
    return;
  }
  void *numeric_address; // Pointer to binary address
  // Buffer to contain result (IPv6 Sufficient to hold IPv4)
  char addr_buffer[INET6_ADDRSTRLEN];
  in_port_t port; // Port to print
  // set pointer to address based on address family
  switch (address->sa_family) {
  case (AF_INET): {
    numeric_address = &((struct sockaddr_in *)address)->sin_addr;
    port = ntohs(((struct sockaddr_in *)address)->sin_port);
    break;
  }
  case (AF_INET6): {
    numeric_address = &((struct sockaddr_in6 *)address)->sin6_addr;
    port = ntohs(((struct sockaddr_in6 *)address)->sin6_port);
    break;
  }
  default : {
      fputs("[unknown type]", stream); // unhandled type
      return;
  }
  }
  
  // Convert binary to printable address
  if (inet_ntop(address->sa_family, numeric_address, addr_buffer, sizeof(addr_buffer)) == NULL) {
      fputs("[invaid address]", stream);
  } else {
      fprintf(stream, "%s", addr_buffer);
      if (port != 0) {
          fprintf(stream, "-%u", port);
      }
  }
}
