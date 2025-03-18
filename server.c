
#include "server.h"



void server() {
    WSADATA wsaData;
    SOCKET server_fd = INVALID_SOCKET;
    SOCKET client_fd[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
    char buffer[1024];
    int nb_clients = 0;
    int result;

    // Initialisation de Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("Echec de l'initialisation WSAStartup: %d\n", result);
        return;
    }

    // Création du socket serveur
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        printf("Echec de la creation du socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Echec du bind: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Echec du listen: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    printf("Hote du jeu demarre sur le port %d...\n", PORT);

    // Initialisation du tableau des sockets clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_fd[i] = INVALID_SOCKET;
    }

    while (1) {
        if (nb_clients < MAX_CLIENTS) {
            SOCKET new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            if (new_socket == INVALID_SOCKET) {
                printf("Echec de l'accept: %d\n", WSAGetLastError());
                continue;
            }

            // Ajouter le nouveau client au tableau
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fd[i] == INVALID_SOCKET) {
                    client_fd[i] = new_socket;
                    break;
                }
            }

            printf("Nouveau joueur connecte : %s\n", inet_ntoa(client_addr.sin_addr));

            nb_clients++;
        }

        // Simulation d'une mise à jour du jeu envoyée aux clients
        strcpy(buffer, "Mise a jour du jeu\n");
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fd[i] != INVALID_SOCKET) {
                send(client_fd[i], buffer, (int)strlen(buffer), 0);
            }
        }

        // Simulation d'une boucle de jeu
        if (nb_clients == 3) {
            printf("nbClient = %d\n", nb_clients);
            while (1) {
                char tmp;
                scanf(" %c", &tmp);
                strcpy(buffer, "a-2");
                printf("Envoie : a = 2\n");
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_fd[i] != INVALID_SOCKET) {
                        send(client_fd[i], buffer, (int)strlen(buffer), 0);
                    }
                }
            }
        }
    }

    // Nettoyage
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fd[i] != INVALID_SOCKET) {
            closesocket(client_fd[i]);
        }
    }
    closesocket(server_fd);
    WSACleanup();
}
