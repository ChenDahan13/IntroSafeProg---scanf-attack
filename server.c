#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 5001  
#define BUFFER_SIZE 1024

int main() {
    signal(SIGPIPE, SIG_IGN);  // on linux to prevent crash on closing socket

    // Opens the listening server socket
    int listeningSocket = -1;
    listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (listeningSocket == -1) {
        printf("Could not create listening socket : %d", errno);
        return 1;
    }
    printf("socket() success\n");

    int enableReuse = 1;
    int ret = setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int));
    if (ret < 0) {
        printf("setsockopt() failed with error code : %d", errno);
        return 1;
    }
    printf("setsockopt() success\n");

    // Create the address of the server
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; // IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // any IP at this port 
    serverAddress.sin_port = htons(SERVER_PORT);  // network order 

    // Bind the socket to the port with any IP at this port
    int bindResult = bind(listeningSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindResult == -1) {
        printf("Bind failed with error code : %d", errno);
        // close the socket
        close(listeningSocket);
        return -1;
    }
    printf("bind() success\n");

    int listenResult = listen(listeningSocket, 1);
    if (listenResult == -1) {
        printf("listen() failed with error code : %d", errno);
        // close the socket
        close(listeningSocket);
        return -1;
    }
    printf("listen() sucssess\n");

    // Accept and incoming connection
    printf("Waiting for incoming TCP-connections...\n");
    struct sockaddr_in clientAddress;  //
    socklen_t clientAddressLen = sizeof(clientAddress);

    while (1) {
        memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientSocket == -1) {
            printf("listen failed with error code : %d", errno);
            // close the sockets
            close(listeningSocket);
            return -1;
        }
        printf("A new client connection accepted\n");
        
        int bytesReceived = 0;
        while(bytesReceived == 0) {
            // Receive a message from sender
            char password[BUFFER_SIZE];
            memset(password, 0, BUFFER_SIZE);
            bytesReceived = recv(clientSocket, password, BUFFER_SIZE, 0);
            if (bytesReceived == -1) {
                printf("recv failed with error code : %d", errno);
                // close the sockets
                close(listeningSocket);
                close(clientSocket);
                return -1;
            }
            printf("Received the password: %s\n", password);   
        }

        
    }
    return 0;
}