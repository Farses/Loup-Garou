#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "client.h"
#include  "server.h"
#include  "font.h"
#include  "jeu.h"
#include  "network.h"

#pragma comment(lib, "ws2_32.lib")



///AJOUTER UN ID A CHAQUE CLIENT
//// COMME CA SI UN CLIENT ENVOI UNE INFO AUX AUTRES, ILS FASSENT PAS L'ACTION
//// AJOUTER AU MSG L'ID DU CLIENT, ET DAUTRES INFO


int main() {
///INITIALISATION DU DISPLAY ET DU TIMER
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *times;
    ALLEGRO_EVENT event = {0};

    srand(time(NULL));

    ///INITIALISATION D'ALLEGROx    x
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ///CREATION DU DISPLAY
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(960, 540);
    al_set_display_flag(display, ALLEGRO_WINDOWED, true);
    double height = al_get_display_height(display);
    double width = al_get_display_width(display);
    al_set_window_position(display, 0, 0);

    ///COULEUR
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    ///FONT
    int size1 = 50 ;
    int size2 = 72 ;
    int size3 = 100 ;
    ALLEGRO_FONT *title_font = NULL;
    ALLEGRO_FONT *subtitle_font = NULL;
    ALLEGRO_FONT *text_font = NULL;
    ALLEGRO_FONT *small_font = NULL;
    load_responsive_fonts(width, height, &title_font, &subtitle_font, &text_font, &small_font);

    ALLEGRO_FONT *gameFont1 = al_load_ttf_font("../Font/Nintendo.ttf", size1, ALLEGRO_ALIGN_LEFT);
    ALLEGRO_FONT *smallGameFont = al_load_ttf_font("../Font/Nintendo.ttf", size2, ALLEGRO_ALIGN_LEFT);

    ///CREATION DES DONNEES DU JEU
    int isFin = 0, draw, compteur = 0;
    bool mouseIsPressed = false ;
    Jeu jeu;
    init(&jeu, width, height) ;

    ///AUDIO
    /*al_install_audio();
    al_init_acodec_addon() ;
    ALLEGRO_SAMPLE *musique = al_load_sample("../Bitmap/musique1.flac");
    al_reserve_samples(1);*/


    ///INITIALISATION DE NOS VARIABLES



    ///INITIALISATION DU TIMER
    times = al_create_timer(0.02);

    ///QUEUE
    queue = al_create_event_queue();
    assert(queue);
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source( ));
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(times));
    al_start_timer(times);

    ///2 BOUCLE WHILE : 1 pour le menu, 1 pour le jeu (conditions dans le while à changer !!)
    while (!isFin) {
        ///MENU DU JEU
        while (jeu.modeJeu == 0 && !isFin) {
            al_wait_for_event(queue, &event);
            switch (event.type) {
                case ALLEGRO_EVENT_KEY_DOWN : {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_ESCAPE : {
                            isFin = 1;
                            break;
                        }
                    }
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_AXES : {
                    jeu.mouse_x = event.mouse.x;
                    jeu.mouse_y = event.mouse.y;
                    break;
                }
                case ALLEGRO_EVENT_DISPLAY_RESIZE : {
                    jeu.width = event.display.width;
                    jeu.height = event.display.height;
                    handle_resize_event(&event, &title_font, &subtitle_font, &text_font, &small_font);
                    al_acknowledge_resize(event.display.source);
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN : {
                    if ((event.mouse.button & 1) == 1) {
                        jeu.mouseIsPressed = true;
                    }
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                    if ((event.mouse.button & 1) == 1) {
                        jeu.mouseIsPressed = false;
                    }
                    break;
                }
                case ALLEGRO_EVENT_TIMER : {
                    draw = 1;
                }
            }
            if (draw) {
                drawChoosing(&jeu, subtitle_font);
                game_event(&jeu) ;
                al_flip_display();
                al_clear_to_color(black);
                draw = 0;
            }
        }

        if ((jeu.modeJeu == 1 || jeu.modeJeu == 2) && !jeu.networkReady) {
            // Show loading screen
            al_clear_to_color(black);
            al_draw_text(subtitle_font, white, jeu.width/2, jeu.height/2, ALLEGRO_ALIGN_CENTER,
                         jeu.modeJeu == 1 ? "Connecting to server..." : "Starting server...");
            al_flip_display();

            // Initialize network
            int success = 0;
            if (jeu.modeJeu == 1) {
                success = initClientMode(&jeu);
            }
            else if (jeu.modeJeu == 2) {
                success = initServerMode(&jeu);
            }

            if (!success) {
                // Connection failed
                al_clear_to_color(black);
                al_draw_text(subtitle_font, white, jeu.width/2, jeu.height/2, ALLEGRO_ALIGN_CENTER,
                             "Connection failed. Press ESC to return to menu.");
                al_flip_display();

                bool backToMenu = false;
                while (!backToMenu && !isFin) {
                    al_wait_for_event(queue, &event);
                    switch (event.type) {
                        case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                            if ((event.mouse.button & 1) == 1) {
                                jeu.mouseIsPressed = false;
                            }
                            break;
                        }
                        case ALLEGRO_EVENT_KEY_DOWN : {
                            switch (event.keyboard.keycode) {
                                case ALLEGRO_KEY_ESCAPE : {
                                    jeu.modeJeu = 0;
                                    backToMenu = true;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }

                continue;
            }
        }

        ///JOUER
        while ((jeu.modeJeu == 1 || jeu.modeJeu == 2) && jeu.networkReady && !isFin) {
            processNetworkMessages(&jeu);
            al_wait_for_event(queue, &event);
            switch (event.type) {
                case ALLEGRO_EVENT_KEY_DOWN : {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_ESCAPE : {
                            isFin = 1;
                            break;
                        }
                    }
                    break;
                }
                case ALLEGRO_EVENT_KEY_UP : {

                    break ;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN : {
                    if ((event.mouse.button & 1) == 1) {
                        jeu.mouseIsPressed = true;
                    }
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                    if ((event.mouse.button & 1) == 1) {
                        jeu.mouseIsPressed = false;
                    }
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_AXES : {
                    jeu.mouse_x = event.mouse.x;
                    jeu.mouse_y = event.mouse.y;
                    break;
                }

                case ALLEGRO_EVENT_TIMER : {
                    draw = 1;
                    compteur++;
                    break;
                }
            }
            if (draw) {
                drawJeu(&jeu, text_font) ;


                al_flip_display();
                al_clear_to_color(black);
                draw = 0;
            }
        }
    }


    if (jeu.isConnected) {
        jeu.isConnected = false;
        Sleep(500); // Give threads time to clean up
    }

    return 0;
}