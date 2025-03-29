#include "tcp_client_utility.hpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
    constexpr size_t MAX_CANDIDATE {1000};

    if (argc < 4 || argc > 5) { // Test for correct # of args
        DieWithUserMessage("Parameter(s)",
            "<Server> <Port/Service> <Candidate> [I]");
    }

    char *server = argv[1];
    char *service = argv[2];
    int candi = atoi(argv[3]);
    if (candi < 0 || candi > MAX_CANDIDATE) {
        DieWithUserMessage("Candidate # not valid", argv[3]);
    }

    bool inq = argc > 4 && strcmp(argv[4], "I") == 0;

    // Create a connected TCP socket
    int sock = SetupTCPClientSocket(server, service);
    if (sock < 0) {
        DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
    }

    FILE *str = fdopen(sock, "r+"); // Wrap for stream I/0
    if (str == NULL) {
        DieWithSystemMessage("fdopen() failed");
    }

    // Set up info for a request
    VoteInfo vi;
    memset(&vi, 0, sizeof(vi));

}