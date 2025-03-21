//
// Created by fares on 19/03/2025.
//

#include "jeu.h"



void init(Jeu* game, float width, float height){
    game->width = width ;
    game->height = height ;
    game->modeJeu=0 ;
    game->mouseIsPressed = false ;
    game->mouse_x= 0 ;
    game->mouse_y= 0 ;
    game->isConnected = false;
    game->networkReady = false;
    game->connectedClients = 0;
    game->requiredClients = 2; // Set to your desired player count requirement
    game->networkState = initNetworkState();
    game->pressedRectangle = false; // Set to your desired player count requirement

}

void game_event(Jeu* game){
    if(game->mouse_x < game->width/2 + 0.182*game->width && game->width/2 + 0.182*game->width > game->width/2 - 0.182*game->width && game->mouse_y < game->height/2 - 0.046*game->height && game->mouse_y > game->height/3) {
        if(game->mouseIsPressed){
            game->modeJeu = 1;
        }
    }

    if(game->mouse_x < game->width/2 + 0.182*game->width && game->width/2 + 0.182*game->width > game->width/2 - 0.182*game->width && game->mouse_y < 2*game->height/3 && game->mouse_y > game->height/2+0.046*game->height) {
        if(game->mouseIsPressed){
            game->modeJeu = 2;
        }
    }
}

void drawChoosing(Jeu* game, ALLEGRO_FONT* font){

    ALLEGRO_COLOR mouse_on_it = al_map_rgb(100, 100, 255);
    ALLEGRO_COLOR mouse_on_it2 = al_map_rgb(100, 100, 255);

    if(game->mouse_x < game->width/2 + 0.182*game->width && game->width/2 + 0.182*game->width > game->width/2 - 0.182*game->width && game->mouse_y < game->height/2 - 0.046*game->height && game->mouse_y > game->height/3) {
        mouse_on_it = al_map_rgb(0, 0, 255);
    }
    else mouse_on_it = al_map_rgb(100, 100, 255);

    if(game->mouse_x < game->width/2 + 0.182*game->width && game->width/2 + 0.182*game->width > game->width/2 - 0.182*game->width && game->mouse_y < 2*game->height/3 && game->mouse_y > game->height/2+0.046*game->height) {
        mouse_on_it2 = al_map_rgb(0, 0, 255);
    }
    else mouse_on_it2 = al_map_rgb(100, 100, 255);

    al_draw_filled_rectangle(game->width/2 - 0.182*game->width, game->height/3, game->width/2 + 0.182*game->width, game->height/2 - 0.046*game->height, mouse_on_it);
    al_draw_filled_rectangle(game->width/2 - 0.182*game->width, game->height/2+0.046*game->height, game->width/2 + 0.182*game->width, 2*game->height/3, mouse_on_it2);

    al_draw_text(font, al_map_rgb(255, 255, 255), game->width/2, game->height/3  +  ((game->height/2-0.046*game->height) - game->height/3)/2 - 0.02*game->height, ALLEGRO_ALIGN_CENTER, "CLIENT");
    al_draw_text(font, al_map_rgb(255, 255, 255), game->width/2, 2*game->height/3 - (2*game->height/3 - (game->height/2+0.046*game->height))/2 - 0.02*game->height, ALLEGRO_ALIGN_CENTER, "SERVER");
}


void drawJeu(Jeu* jeu, ALLEGRO_FONT* text_font){
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255) ;
    if (jeu->isConnected) {
        al_draw_text(text_font, white, 20, 20, ALLEGRO_ALIGN_LEFT,
                     jeu->modeJeu == 1 ? "Connecte au server" : "Server running");

        char playerCountText[100];
        sprintf(playerCountText, "Joueurs %d sur %d", jeu->connectedClients, jeu->requiredClients);
        al_draw_text(text_font, white, 20, 60, ALLEGRO_ALIGN_LEFT, playerCountText);


        if (jeu->connectedClients >= jeu->requiredClients) {

            al_draw_text(text_font, al_map_rgb(0, 255, 0), jeu->width/2, 100, ALLEGRO_ALIGN_CENTER,
                         "Tout les joueurs sont connectes ! La partie peut commencer.");

            if (jeu->modeJeu == 2) { /// Server mode
                al_draw_filled_rectangle(jeu->width/2 - 100, 150, jeu->width/2 + 100, 200, al_map_rgb(0, 100, 0));
                al_draw_text(text_font, white, jeu->width/2, 165, ALLEGRO_ALIGN_CENTER, "Start Game");


                if (jeu->mouseIsPressed && jeu->mouse_x >= jeu->width/2 - 100 && jeu->mouse_x <= jeu->width/2 + 100 && jeu->mouse_y >= 150 && jeu->mouse_y <= 200) {

                    ///LANCER LE JEU ICI

                }
            }
            else { /// Client mode
                al_draw_text(text_font, white, jeu->width/2, 150, ALLEGRO_ALIGN_CENTER,
                             "Attente du server qui lance le jeu...");
                al_draw_filled_rectangle(100, 250, 300, 400, al_map_rgb(125, 60, 140)) ;

                if (jeu->mouse_x >= 100 && jeu->mouse_x <= 300 && jeu->mouse_y >= 250 && jeu->mouse_y <= 400) {
                    al_draw_filled_rectangle(100, 250, 300, 400, al_map_rgb(10, 10, 180)) ;

                    if(jeu->mouseIsPressed) {
                        unsigned char data[1] = {0xFF};

                        queueMessage(jeu->networkState, MSG_TYPE_GAME_UPDATE, data, 1);
                    }
                }
                if (jeu->pressedRectangle) {
                    al_draw_text(text_font, al_map_rgb(255, 255, 0), jeu->width/2, 400, ALLEGRO_ALIGN_CENTER,
                                 "APPUYER !!!!");
                }
            }
        } else {
            // Not enough players yet
            al_draw_text(text_font, al_map_rgb(255, 255, 0), jeu->width/2, 100, ALLEGRO_ALIGN_CENTER,
                         "Attente d'autres joueurs a se connecter...");
        }
    }
    else {
        al_draw_text(text_font, al_map_rgb(255, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT,
                     "Connexion perdu");
    }
}

