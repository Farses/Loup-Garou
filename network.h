#ifndef AAAAAAAAAA_NETWORK_H
#define AAAAAAAAAA_NETWORK_H

#include "game.h"


///QUEUE DE MESSAGES
void initMessageQueue(MessageQueue* queue) ;
int enqueueMessage(MessageQueue* queue, unsigned char type, const unsigned char* data, int dataLength) ;
int dequeueMessage(MessageQueue* queue, NetworkMessage* msg) ;
void cleanupMessageQueue(MessageQueue* queue) ;
void queueMessage(NetworkState* netState, unsigned char type, const unsigned char* data, int dataLength) ;
///STRUCTURE NETWORK STATE (Incomming et outgoing queues)
void cleanupNetworkState(NetworkState* netState) ;
NetworkState* initNetworkState() ;
void processNetworkMessages(Jeu* game) ;

///ENVOYER UN MESSAGE
int sendMessage(SOCKET sock, unsigned char type, const unsigned char* data, int dataLength) ;
void sendQueuedMessages(NetworkState* netState, SOCKET sock) ;
void sendQueuedMessagesToAllClients(NetworkState* netState, SOCKET* clientsockets) ;

///RECEVOIR LES MESSAGES
void handleIncomingMessage(NetworkState* netState, char* buffer, int length) ;

#endif //AAAAAAAAAA_NETWORK_H
