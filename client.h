//
// Created by fares on 18/03/2025.
//

#ifndef AAAAAAAAAA_CLIENT_H
#define AAAAAAAAAA_CLIENT_H

#include "game.h"
#include "network.h"

DWORD WINAPI client_receive_thread(LPVOID data) ;
int initClientMode(Jeu* game) ;
void handleReceivedMessage(Jeu* game, char* buffer, int length) ;
#endif //AAAAAAAAAA_CLIENT_H
