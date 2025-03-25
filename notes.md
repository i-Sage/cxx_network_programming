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