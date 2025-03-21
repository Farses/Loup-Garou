#include "client.h"


// Initialize client connection and create a listening thread
int initClientMode(Jeu* game) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("Failed to initialize WSAStartup: %d\n", result);
        return 0;
    }

    // Create client socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Failed to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Failed to connect: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    printf("Connected to server!\n");
    game->isConnected = true;

    // Create thread to handle receiving messages
    HANDLE thread_handle;
    NetworkThreadData* threadData = (NetworkThreadData*)malloc(sizeof(NetworkThreadData));
    threadData->game = game;
    threadData->sock = sock;

    thread_handle = CreateThread(NULL, 0, client_receive_thread, threadData, 0, NULL);
    if (thread_handle == NULL) {
        printf("Failed to create client thread\n");
        closesocket(sock);
        WSACleanup();
        free(threadData);
        return 0;
    }

    // We don't need to keep the handle since the thread will clean itself up
    CloseHandle(thread_handle);

    game->networkReady = true;
    return 1;
}


DWORD WINAPI client_receive_thread(LPVOID data) {
    NetworkThreadData* threadData = (NetworkThreadData*)data;
    Jeu* game = threadData->game;
    SOCKET sock = threadData->sock;
    NetworkState* netState = game->networkState;  // Assume you've added this to Jeu struct

    char buffer[1024];
    int result;

    if (!netState) {
        game->networkState = initNetworkState();
        netState = game->networkState;
    }
    while (game->isConnected) {
        unsigned char datafornothing[1] = { 0x00} ;
        queueMessage(netState, MSG_TYPE_NOTHING, datafornothing, 1 ) ;
        memset(buffer, 0, sizeof(buffer));
        result = recv(sock, buffer, sizeof(buffer), 0);
        if (result > 0) {
            handleIncomingMessage(netState, buffer, result);
        }
        else if (result == 0) {
            printf("Connection closed by server\n");
            break;
        }
        else {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK) {
                printf("recv failed: %d\n", err);
                break;
            }
        }


        sendQueuedMessages(netState, sock);
        // Small sleep to prevent CPU hogging
        Sleep(5);
    }

    closesocket(sock);
    game->isConnected = false;
    free(threadData);
    return 0;
}