// Client with private messaging
#include <winsock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <process.h>
#include <stdbool.h>

#pragma comment(lib, "ws2_32.lib")

bool initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

unsigned __stdcall SendMsg(void* args) {
    SOCKET s = *((SOCKET*)args);
    char message[4096];
    char name[100];

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    send(s, name, (int)strlen(name), 0);

    printf("Enter password: ");
    char password[100];
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    send(s, password, (int)strlen(password), 0);

    while (true) {
        // Clear the message buffer
        memset(message, 0, sizeof(message));

        // Read input character by character
        int i = 0;
        while ((message[i] = getchar()) != '\n' && i < sizeof(message) - 1) {
            i++;
        }
        message[i] = '\0';

        if (strncmp(message, "quit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        send(s, message, (int)strlen(message), 0);
    }

    return 0;
}

unsigned __stdcall ReceiveMsg(void* args) {
    SOCKET s = *((SOCKET*)args);
    char buffer[4096];
    int recvlength;

    while (true) {
        recvlength = recv(s, buffer, sizeof(buffer), 0);
        if (recvlength <= 0) {
            printf("Disconnected from the server.\n");
            break;
        }
        else {
            buffer[recvlength] = '\0';
            printf("\r%s\n", buffer);
            // Print a fresh prompt (optional)
            printf("> ");
            fflush(stdout);
        }
    }

    return 0;
}

int main() {
    if (!initialize()) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

    connect(s, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    HANDLE senderThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, &s, 0, NULL);
    HANDLE receiverThread = (HANDLE)_beginthreadex(NULL, 0, ReceiveMsg, &s, 0, NULL);

    WaitForSingleObject(senderThread, INFINITE);
    WaitForSingleObject(receiverThread, INFINITE);

    CloseHandle(senderThread);
    CloseHandle(receiverThread);
    closesocket(s);
    WSACleanup();
    return 0;
}