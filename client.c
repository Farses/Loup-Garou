#include "client.h"



char a; // Déclaration de la variable globale 'a'

void client() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int result;

    // Initialisation de Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("Echec de l'initialisation WSAStartup: %d\n", result);
        return;
    }

    // Création du socket client
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Echec de la creation du socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Echec de la connexion: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return;
    }

    printf("Connecte a l'hote !\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        result = recv(sock, buffer, sizeof(buffer), 0);
        if (result > 0) {
            printf("Message recu : %s", buffer);
            if (buffer[0] == 'a' && buffer[1] == '-') {
                a = buffer[2];
                printf("Modification de a = %c\n", a);
            }
            else if (buffer[0] == 'M' && buffer[1] == 'i') {
                printf("Mise a jour recue : %s\n", buffer);
            }
            else {
                printf("Condition non remplie\n");
            }
        }
        else if (result == 0) {
            printf("Connexion fermee par le serveur\n");
            break;
        }
        else {
            printf("recv a echoue: %d\n", WSAGetLastError());
            break;
        }
    }
    closesocket(sock);
    WSACleanup();
}


DWORD WINAPI client_receive_thread(LPVOID data) {
    NetworkThreadData* threadData = (NetworkThreadData*)data;
    Jeu* game = threadData->game;
    SOCKET sock = threadData->sock;
    char buffer[1024];
    int result;

    while (game->isConnected) {
        memset(buffer, 0, sizeof(buffer));
        result = recv(sock, buffer, sizeof(buffer), 0);
        if (result > 0) {
            printf("Message received: %s", buffer);
            if (buffer[0] == 'a' && buffer[1] == '-') {
                game->a = buffer[2];
                printf("Modified a = %c\n", game->a);
            }
            else if (buffer[0] == 'M' && buffer[1] == 'i') {
                printf("Update received: %s\n", buffer);
                // Handle game state updates here
            }
        }
        else if (result == 0) {
            printf("Connection closed by server\n");
            break;
        }
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(sock);
    game->isConnected = false;
    free(threadData);
    return 0;
}

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
