#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 5001
#define SERVER_IP_ADDRESS "127.0.0.1"

#define BUFFER_SIZE 1024


int scanf(const char* format, ...) {
    char password[20];
    gets(password);

    // Opens a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == -1) {
        printf("Could not create socket : %d\n", errno);
        return -1;
    }
    
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);                                              
    int rval = inet_pton(AF_INET, (const char *)SERVER_IP_ADDRESS, &serverAddress.sin_addr);  
    if (rval <= 0) {
        printf("inet_pton() failed\n");
        return -1;
    }

    // Makes a connection to the server with socket SendingSocket.
    int connectResult = connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectResult == -1) {
        printf("connect() failed with error code : %d\n", errno);
        // cleanup the socket;
        close(sock);
        return -1;
    }

    sleep(5);
    // Sends the password to server
    int passwordLen = strlen(password) + 1;
    int bytesSent = send(sock, password, passwordLen, 0);
    if (bytesSent == -1) {
        printf("send() failed with error code : %d\n", errno);
    } else if (bytesSent == 0) {
        printf("peer has closed the TCP connection prior to send().\n");
    } else if (bytesSent < passwordLen) {
        printf("sent only %d bytes from the required %d.\n", passwordLen, bytesSent);
    }
    
    
    close(sock);
    return 0;
}


    

