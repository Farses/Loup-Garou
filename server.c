
#include "server.h"


DWORD WINAPI server_thread(LPVOID data) {
    NetworkThreadData* threadData = (NetworkThreadData*)data;
    Jeu* game = threadData->game;
    SOCKET* client_fds = threadData->client_fds;
    int* nb_clients = threadData->nb_clients;

    WSADATA wsaData;
    SOCKET server_fd = INVALID_SOCKET;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
    char buffer[1024];
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("Failed to initialize WSAStartup: %d\n", result);
        goto cleanup;
    }

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        printf("Failed to create socket: %d\n", WSAGetLastError());
        WSACleanup();
        goto cleanup;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Failed to bind: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        goto cleanup;
    }

    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Failed to listen: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        goto cleanup;
    }

    printf("Game server started on port %d...\n", PORT);
    game->isConnected = true;

    // Make server socket non-blocking to handle game updates
    u_long mode = 1;
    ioctlsocket(server_fd, FIONBIO, &mode);

    while (game->isConnected) {
        // Accept new clients if there's room
        if (*nb_clients < MAX_CLIENTS) {
            SOCKET new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            if (new_socket != INVALID_SOCKET) {
                // Add new client to array
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_fds[i] == INVALID_SOCKET) {
                        client_fds[i] = new_socket;
                        break;
                    }
                }

                printf("New player connected: %s\n", inet_ntoa(client_addr.sin_addr));
                (*nb_clients)++;

                // Send initial game state
                sprintf(buffer, "Mise a jour du jeu initial\n");
                send(new_socket, buffer, (int)strlen(buffer), 0);
            }
        }

        // Send periodic game updates
        if (*nb_clients > 0 && game->networkReady) {
            sprintf(buffer, "Mise a jour du jeu\n");
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] != INVALID_SOCKET) {
                    send(client_fds[i], buffer, (int)strlen(buffer), 0);
                }
            }
        }

        // Sleep to avoid consuming too much CPU
        Sleep(100);
    }

    // Cleanup
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] != INVALID_SOCKET) {
            closesocket(client_fds[i]);
        }
    }
    closesocket(server_fd);
    WSACleanup();

    cleanup:
    free(client_fds);
    free(nb_clients);
    free(threadData);
    return 0;
}


// Function to send game updates from server to all clients
void sendGameUpdate(Jeu* game, SOCKET* client_fds, int nb_clients, const char* update) {
    if (!game->isConnected || !game->networkReady || game->modeJeu != 2) {
        return;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] != INVALID_SOCKET) {
            send(client_fds[i], update, (int)strlen(update), 0);
        }
    }
}

// Initialize server mode and create a server thread
int initServerMode(Jeu* game) {
    HANDLE thread_handle;
    SOCKET* client_fds = (SOCKET*)malloc(MAX_CLIENTS * sizeof(SOCKET));
    int* nb_clients = (int*)malloc(sizeof(int));

    *nb_clients = 0;

    // Initialize client sockets array
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_fds[i] = INVALID_SOCKET;
    }

    NetworkThreadData* threadData = (NetworkThreadData*)malloc(sizeof(NetworkThreadData));
    threadData->game = game;
    threadData->client_fds = client_fds;
    threadData->nb_clients = nb_clients;

    thread_handle = CreateThread(NULL, 0, server_thread, threadData, 0, NULL);
    if (thread_handle == NULL) {
        printf("Failed to create server thread\n");
        free(client_fds);
        free(nb_clients);
        free(threadData);
        return 0;
    }

    // We don't need to keep the handle since the thread will clean itself up
    CloseHandle(thread_handle);

    game->networkReady = true;
    return 1;
}

