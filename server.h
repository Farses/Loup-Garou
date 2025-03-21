

#ifndef AAAAAAAAAA_SERVER_H
#define AAAAAAAAAA_SERVER_H


#include "game.h"
#include "network.h"


DWORD WINAPI server_thread(LPVOID data) ;
int initServerMode(Jeu* game) ;
void updateClientCount(Jeu* game, int count) ;


///SENDING FUNCTIONS
void sendPlayerCount(NetworkState* netState, int count) ;

#endif //AAAAAAAAAA_SERVER_H
