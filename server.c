// Server with private messaging
#include <winsock2.h>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 4096
#define SERVER_PASSWORD "WCHAT5"

typedef struct {
    SOCKET socket;
    char name[100];
} Client;

CRITICAL_SECTION clientsCriticalSection;
Client clients[FD_SETSIZE];
int clientCount = 0;

BOOL Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

unsigned __stdcall InteractWithClient(void* arg) {
    SOCKET clientSocket = *(SOCKET*)arg;
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    char clientName[100];

    // Request client name
    send(clientSocket, "Enter your name: ", 17, 0);
    bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return 0;
    }
    buffer[bytesReceived] = '\0';
    strcpy_s(clientName, sizeof(clientName), buffer);

    EnterCriticalSection(&clientsCriticalSection);
    strcpy_s(clients[clientCount].name, sizeof(clients[clientCount].name), clientName);
    clients[clientCount].socket = clientSocket;
    clientCount++;
    LeaveCriticalSection(&clientsCriticalSection);

    printf("Client [%s] connected\n", clientName);

    // Request password
    send(clientSocket, "Enter password: ", 16, 0);
    bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    buffer[bytesReceived] = '\0';

    if (strcmp(buffer, SERVER_PASSWORD) != 0) {
        send(clientSocket, "Invalid password\n", 17, 0);
        closesocket(clientSocket);
        return 0;
    }

    send(clientSocket, "Welcome to the chat!\n", 21, 0);

    while (1) {
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            printf("Client [%s] disconnected\n", clientName);
            break;
        }

        buffer[bytesReceived] = '\0';
        printf("[%s]: %s\n", clientName, buffer);

        // Private messaging logic
        if (buffer[0] == '/') {
            char recipientName[100], message[BUFFER_SIZE];
            int result = sscanf_s(buffer, "/%99s %[^\n]", recipientName, sizeof(recipientName), message, sizeof(message));

            if (result == 2) {
                char privateMessage[BUFFER_SIZE + 100];
                sprintf_s(privateMessage, sizeof(privateMessage), "[Private from %s]: %s", clientName, message);

                EnterCriticalSection(&clientsCriticalSection);
                for (int i = 0; i < clientCount; ++i) {
                    if (strcmp(clients[i].name, recipientName) == 0) {
                        send(clients[i].socket, privateMessage, (int)strlen(privateMessage), 0);
                        break;
                    }
                }
                LeaveCriticalSection(&clientsCriticalSection);
            }
        }
        else {
            // Broadcast message to all except sender
            char broadcastMessage[BUFFER_SIZE + 100];
            sprintf_s(broadcastMessage, sizeof(broadcastMessage), "[%s]: %s", clientName, buffer);

            EnterCriticalSection(&clientsCriticalSection);
            for (int i = 0; i < clientCount; ++i) {
                if (clients[i].socket != clientSocket) {
                    send(clients[i].socket, broadcastMessage, (int)strlen(broadcastMessage), 0);
                }
            }
            LeaveCriticalSection(&clientsCriticalSection);
        }
    }

    closesocket(clientSocket);
    _endthreadex(0);
    return 0;
}

int main() {
    if (!Initialize()) {
        printf("Winsock initialization failed\n");
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    InitializeCriticalSection(&clientsCriticalSection);
    printf("Server started, listening on port %d\n", PORT);

    while (1) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        EnterCriticalSection(&clientsCriticalSection);
        clients[clientCount].socket = clientSocket;
        LeaveCriticalSection(&clientsCriticalSection);

        _beginthreadex(NULL, 0, InteractWithClient, &clientSocket, 0, NULL);
    }

    DeleteCriticalSection(&clientsCriticalSection);
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}