## Creating and Destroying Sockets
If we want to communicate using TCP or UDP, a program begins by asking the OS to create an instance of the socket abstraction. The function to perform this task is `socket()`; its parameters specify the flavor of the socket needed by our program:
```cpp
int socket(int domain, int type, int protocol);
```
- Parameter 1 determines the communication domain of the socket. The socket API provides a generic interface for a large number of communication domains; however we are interested in IPv4 (`AF_INET`) and IPv6(`AF_INET6`).

- Parameter 2 specifies the type of socket. The type determines the semantics of data transmission with the socket, i.e., if the transmission is reliable, if message boundaries are preserved, and so on. The constant `SOCK_STREAM` specifies a socket with reliable byte-stream semantics, where as `SOCK_DGRAM` specifies a best-effort datagram socket.

- Parameter 3 specifies the particular end-to-end protocol to be used. For both IPv4 and IPv6, we want TCP (identified by the constant `IPPROTO_TCP`) for a stream socket, or UDP (identified by `IPPROTO_UDP`) for a datagram socket. Supplying the constant 0 as the third parameter causes the system to select the default end-to-end protocol for the specified protocol family and type.

The socket function returns a handle for the communication instance. On unix-derived systems, it is an integer: a nonnegative value for success, and -1 for failre. A nonfailure value should be treated as an opaque handle, like a file descriptor. This handle, which is known as a `socket descriptor`, is passed to other API functions to identify the socket abstraction on which the operation is to be carried out.

```cpp
int close(int socket)
```
close() tells the underlying protocol stack to initiate any actions required to shut down communications and deallocate any resources associated with the socket. close() returns 0 on success or -1 on failure. Once close() has been called, invoking other operations like send() and recv() on the socket results in an error.


## Part 1 Specifying Addresses
#### 1. Generic Addresses
The sockets API defines a generic data tpe -- the sockaddr structure -- for specifying addresses associated with sockets:

```cpp
struct sockaddr {
    sa_family sa_family;         // Address family (e.g., AF_INET)
    char sa_data[14];            // Family specific address information
};
```

#### 2. Ipv4 Addresses
The particular form of the `sockaddr` structure that is used for TCP/IP socket addresses depends on the IP version. For IPv4, use the `sockaddr_in` struct

```cpp
struct in_addr{
    uint32_t s_addr;   // Internet address (32 bits)
};

struct sockaddr_in {
    sa_family_t sin_family;    // Internet protocol (AF_INET)
    in_port_t sin_port;        // Address port (16 bits)
    struct in_addr sin_addr;   // IPv4 address (32 bits)
    char sin_zero[8];          // Not used
}
```

#### 3. Binary/String Address Conversion
For socket functions to understand addresses, they must be in "numeric" (i.e., binary) form; however, addresses for human use are generally "printable" strings. We can convert addresses from printable strings to numeric using the inet_pton() rundtion (pton= printable to numeric):

```cpp
int inet_pton(int addressfamily, const char *src, void *dst);
```

returns 1 if the conversion is successful, 0 if the string pointed to by src is not formatted as a valid address; and -1 if the specified address family is unknown.

We can go the other way, converting addresses from numeric to printiable form, using inet_ntop() (ntop = numeric to printable)

```cpp
const char *inet_ntop(int addressfamily, const void *src, char *dst, socklen_t dstbyest)
```

#### 4. Getting a Socket's Associated Addresses
The system associates a local and foreign address with each connected socket (TCP or UDP). We can find out these addresses using `getsockname()` for the local address and `getperrname()` for the foreign address. Both methods return a `sockaddr` structure containing the internet address and port information

```cpp
int getpeername(int socket, struct sockaddr *remote_address, socketlen_t *address_length)
int getsockname(int socket, struct sockaddr *local_address, socketlen_t *address_length)
```
The socket parameter is the descriptor of the socket whose address information we want. The remote_address and local address_parameters point to address structures into which the address information will be plaed by the implementation; they are always case to `sockaddr *` by the caller. As with other socket calls using  `sockaddr`, the address_length is an in-out parameter specifying the length of the buffer(input) and returned address structre (output) in bytes.

## Part 2: Connecting a socket
A TCP socket must be connected to another socket before any data can be sent through it. In this sense using TCP sockets is something like using the telephone network. Before you can talk, you have to specify the number you want, and a connection must be established; if the connection cannot be established, you have to try again later. The client initiaites the connection while the server waits passively for clients to connect to it. To establish a connection with a server, we call `connect()` on the socket:
```cpp
int connect(int socket, const struct sockaddr *foreign_addr, socklen_t addr_len)
```

### Part 3: Binding to an Address
