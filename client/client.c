#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFLEN 2048
#define PORT 3000

int main() {
    int sock, addressLength, n;
    char buf[BUFLEN], message[BUFLEN];
    struct sockaddr_in addr;
        
    printf("init...");
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    addressLength = sizeof(addr);

    char * clientConnectedMessage = "Client connected";
    n = sendto(sock, clientConnectedMessage, strlen(clientConnectedMessage), 0, (struct sockaddr *)&addr, addressLength);
    if (n < 0) {
        printf("Error in send data");
    }

    printf("\n\n");
    printf("Enter message: ");
    gets(message);

    if (strcmp(message, ":exit") == 0) {
        return 0;
    }

    n = sendto(sock, message, sizeof(message), 0, (struct sockaddr *)&addr, addressLength);
    if (n < 0) {
        printf("Error in send data");
        return 1;
    }

    memset(buf, '\0', BUFLEN);

    n = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&addr, &addressLength);
    if (n < 0) {
        printf("Error in rec data\n");
        return 1;
    }
    buf[n] = '\0';

    printf("Result: ");
    printf("\n");
    printf(buf);

    char * messageReceived = "Message received";
    n = sendto(sock, messageReceived, strlen(messageReceived), 0, (struct sockaddr *)&addr, addressLength);
    if (n < 0) {
        printf("Error in send data");
        return 1;
    }
    
    close(sock);
    
    return 0;
}
