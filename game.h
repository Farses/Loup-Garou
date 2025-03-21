
#ifndef AAAAAAAAAA_GAME_H
#define AAAAAAAAAA_GAME_H



///INCLUDE
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///DEFINE
#define PORT 8080
#define MAX_CLIENTS 2
#define PI 3.14159265358979323846
#define MSG_TYPE_VARIABLE_UPDATE 0x01  // For variable updates like 'a'
#define MSG_TYPE_GAME_UPDATE     0x02  // For game state updates
#define MSG_TYPE_PLAYER_COUNT    0x03  // For player count updates
#define MSG_TYPE_CUSTOM_COMMAND  0x04  // For custom commands
#define MSG_TYPE_NOTHING  0x05  // For custom commands

#define MAX_QUEUE_SIZE 32
#define MAX_MESSAGE_SIZE 1024

///STRUCTURE ET ENUM
enum ROLE{VILLAGEOIS, LOUP, SORCIERE, PETITE_FILLE, MEDECIN};


typedef struct{
    float dixieme ;
    int secondes, minutes ;
} Temps ;




typedef struct {
    unsigned char type;            // Message type
    unsigned char data[MAX_MESSAGE_SIZE];  // Message data
    int dataLength;                // Length of data
} NetworkMessage;

typedef struct {
    NetworkMessage messages[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
    CRITICAL_SECTION cs;  // For thread safety
} MessageQueue;

typedef struct {
    MessageQueue incomingMessages;  // Messages received from network
    MessageQueue outgoingMessages;  // Messages to send over network
    // ... other game state fields
} NetworkState;

typedef struct {
    float width, height ;       // Info écran
    int mouse_x, mouse_y ;
    bool mouseIsPressed;

    //TEMPS
    Temps time[2];

    //BITMAP

    // MODE DE JEU
    int modeJeu;

    // Network
    bool isConnected;
    bool networkReady;
    int connectedClients;
    int requiredClients;
    NetworkState* networkState;


    bool pressedRectangle ;



    char a;  // Your global variable from client.c

} Jeu ;

typedef struct {
    Jeu* game;
    SOCKET sock;  // For client mode
    SOCKET* client_fds;  // For server mode
    int* nb_clients;
} NetworkThreadData;


#endif //AAAAAAAAAA_GAME_H
