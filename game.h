
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



///STRUCTURE ET ENUM
enum ROLE{VILLAGEOIS, LOUP, SORCIERE, PETITE_FILLE, MEDECIN};


typedef struct{
    float dixieme ;
    int secondes, minutes ;
} Temps ;


typedef struct {
    float width, height ;       // Info écran
    int mouse_x, mouse_y ;
    bool mouseIsPressed;

    //STRUCTURE


    //GRILLE


    //TEMPS
    Temps time[2];


    //BITMAP


    // MODE DE JEU ?
    int modeJeu;
} Jeu ;

#endif //AAAAAAAAAA_GAME_H
