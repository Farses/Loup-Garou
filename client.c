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
