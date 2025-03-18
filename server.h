

#ifndef AAAAAAAAAA_SERVER_H
#define AAAAAAAAAA_SERVER_H


#include "game.h"


void sendGameUpdate(Jeu* game, SOCKET* client_fds, int nb_clients, const char* update) ;
DWORD WINAPI server_thread(LPVOID data) ;
int initServerMode(Jeu* game) ;
void updateClientCount(Jeu* game, int count) ;
void broadcastClientCount(Jeu* game, SOCKET* client_fds, int nb_clients) ;

#endif //AAAAAAAAAA_SERVER_H
