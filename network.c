#include "network.h"


///QUEUE DE MESSAGE ---- CREATION ---- AJOUT A LA QUEUE ---- VIDANGE DE LA QUEUE ----

// Initialize message queue
void initMessageQueue(MessageQueue* queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    InitializeCriticalSection(&queue->cs);
}

// Add message to queue
int enqueueMessage(MessageQueue* queue, unsigned char type, const unsigned char* data, int dataLength) {
    EnterCriticalSection(&queue->cs);

    if (queue->count >= MAX_QUEUE_SIZE) {
        LeaveCriticalSection(&queue->cs);
        return 0;  // Queue full
    }

    // Copy message data
    NetworkMessage* msg = &queue->messages[queue->tail];
    msg->type = type;
    if (data && dataLength > 0) {
        int copyLength = dataLength < MAX_MESSAGE_SIZE ? dataLength : MAX_MESSAGE_SIZE;
        memcpy(msg->data, data, copyLength);
        msg->dataLength = copyLength;
    } else {
        msg->dataLength = 0;
    }

    // Update queue
    queue->tail = (queue->tail + 1) % MAX_QUEUE_SIZE;
    queue->count++;

    LeaveCriticalSection(&queue->cs);
    return 1;
}

void queueMessage(NetworkState* netState, unsigned char type, const unsigned char* data, int dataLength) {
    enqueueMessage(&netState->outgoingMessages, type, data, dataLength);
}

// Get next message from queue
int dequeueMessage(MessageQueue* queue, NetworkMessage* msg) {
    EnterCriticalSection(&queue->cs);

    if (queue->count <= 0) {
        LeaveCriticalSection(&queue->cs);
        return 0;  // Queue empty
    }

    // Copy message
    *msg = queue->messages[queue->head];

    // Update queue
    queue->head = (queue->head + 1) % MAX_QUEUE_SIZE;
    queue->count--;

    LeaveCriticalSection(&queue->cs);
    return 1;
}

void cleanupMessageQueue(MessageQueue* queue) {
    DeleteCriticalSection(&queue->cs);
}

///RECEVOIR
void handleIncomingMessage(NetworkState* netState, char* buffer, int length) {
    if (length < 1) return;  // Invalid message

    // Check if hex format
    if (buffer[0] & 0x80) {
        unsigned char msgType = buffer[0] & 0x7F;
        int dataOffset = 3;


        // Validate data length
        enqueueMessage(&netState->incomingMessages, msgType,
                           (unsigned char*)buffer + dataOffset, length - dataOffset);

    }

    else {
        // Convert text format to our message format
        if (buffer[0] == 'a' && buffer[1] == '-' && length >= 3) {
            unsigned char data[1] = { buffer[2] };
            enqueueMessage(&netState->incomingMessages, MSG_TYPE_VARIABLE_UPDATE, data, 1);
        }
        else if (strncmp(buffer, "Mise a jour", 11) == 0) {
            enqueueMessage(&netState->incomingMessages, MSG_TYPE_GAME_UPDATE,
                           (unsigned char*)buffer, length);
        }
        else if (strncmp(buffer, "PlayerCount:", 12) == 0) {
            int count = atoi(buffer + 12);
            unsigned char data[1] = { (unsigned char)count };
            enqueueMessage(&netState->incomingMessages, MSG_TYPE_PLAYER_COUNT, data, 1);
        }
    }
}


/// ENVOI DE MESSAGES GRÂCE A LA QUEUE

//NETWORK STATE : COMPREND LA QUEUE DE MESSAGE A ENVOYER/A RECEVOIR
NetworkState* initNetworkState() {
    NetworkState* netState = (NetworkState*)malloc(sizeof(NetworkState));
    if (netState) {
        initMessageQueue(&netState->incomingMessages);
        initMessageQueue(&netState->outgoingMessages);
    }
    return netState;
}

// Clean up network state
void cleanupNetworkState(NetworkState* netState) {
    if (netState) {
        cleanupMessageQueue(&netState->incomingMessages);
        cleanupMessageQueue(&netState->outgoingMessages);
        free(netState);
    }
}

//ENVOYER UN MESSAGE (UN SEUL)
int sendMessage(SOCKET sock, unsigned char type, const unsigned char* data, int dataLength) {
    // Allocate buffer for message
    int headerSize = 1;  // Type byte
    if (dataLength > 0) {
        headerSize += 2;  // Add length bytes
    }

    unsigned char* buffer = (unsigned char*)malloc(headerSize + dataLength);
    if (!buffer) return -1;

    // Set message type with format bit
    buffer[0] = type | 0x80;

    // Set data length if needed
    ///un peu comme du little endian : taille sur 2 bytes, le 1er octet en buffer[1], le 2ème octer en buffer[2]
    if (dataLength > 0) {
        buffer[1] = (unsigned char)(dataLength & 0xFF);
        buffer[2] = (unsigned char)((dataLength >> 8) & 0xFF);
        memcpy(buffer + headerSize, data, dataLength);
    }

    // Send message
    int result = send(sock, (char*)buffer, headerSize + dataLength, 0);
    free(buffer);
    return result;
}


//ENVOYER TOUTE LA QUEUE
void sendQueuedMessages(NetworkState* netState, SOCKET sock) {
    NetworkMessage msg;
    while (dequeueMessage(&netState->outgoingMessages, &msg)) {
        sendMessage(sock, msg.type, msg.data, msg.dataLength);
    }
}

void sendQueuedMessagesToAllClients(NetworkState* netState, SOCKET* clientsockets) {
    NetworkMessage msg;
    while (dequeueMessage(&netState->outgoingMessages, &msg)) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientsockets[i] != INVALID_SOCKET) {
                sendMessage(clientsockets[i], msg.type, msg.data, msg.dataLength);
            }
        }
    }
}


//RECEVOIR TOUTE LA QUEUE
void processNetworkMessages(Jeu* game) {
    NetworkState* netState = game->networkState;
    NetworkMessage msg;

    // Process all incoming messages
    while (dequeueMessage(&netState->incomingMessages, &msg)) {
        switch (msg.type) {
            case MSG_TYPE_VARIABLE_UPDATE: {


                break;
            }

            case MSG_TYPE_GAME_UPDATE: {

                if(msg.data[0] ==0xFF){
                    game->pressedRectangle = true ;
                }

                break;
            }

            case MSG_TYPE_PLAYER_COUNT: {
                if (msg.dataLength >= 1) {
                    game->connectedClients = msg.data[0];
                }
                break;
            }

            case MSG_TYPE_CUSTOM_COMMAND: {
                printf("Custom command received\n");


                break;
            }
            case MSG_TYPE_NOTHING: {

                //RIEN FAIRE
                break;
            }

        }
    }
}


