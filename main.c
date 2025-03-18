#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "client.h"
#include  "server.h"



#pragma comment(lib, "ws2_32.lib")



int main() {
///INITIALISATION DU DISPLAY ET DU TIMER
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *times;
    ALLEGRO_EVENT event = {0};

    srand(time(NULL));

    ///INITIALISATION D'ALLEGRO
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ///CREATION DU DISPLAY
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(800, 600);
    //al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
    double height = al_get_display_height(display);
    double width = al_get_display_width(display);
    al_set_window_position(display, 0, 0);

    ///COULEUR
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    ///FONT
    ALLEGRO_FONT *bigGameFont = al_load_ttf_font("../Font/Nintendo.ttf", 300, ALLEGRO_ALIGN_CENTER);
    ALLEGRO_FONT *gameFont1 = al_load_ttf_font("../Font/Nintendo.ttf", 3*width/80, ALLEGRO_ALIGN_LEFT);
    ALLEGRO_FONT *smallGameFont = al_load_ttf_font("../Font/Nintendo.ttf", 5 * width / 192, ALLEGRO_ALIGN_LEFT);
    ALLEGRO_FONT *gameFont = al_load_ttf_font("../Font/MagicCardsNormal.ttf", 2 * width / 55, ALLEGRO_ALIGN_LEFT);
    ALLEGRO_FONT *gameFontRegles = al_load_ttf_font("../Font/Rumpi.ttf", 40, ALLEGRO_ALIGN_LEFT);

    ///CREATION DES DONNEES DU JEU
    int isFin = 0, draw, compteur = 0;
    bool mouseIsPressed = false ;
    Jeu jeu;
    jeu.width = width ;
    jeu.height = height ;
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
    al_register_event_source(queue, al_get_timer_event_source(times));
    al_start_timer(times);

    ///2 BOUCLE WHILE : 1 pour le menu, 1 pour le jeu (conditions dans le while à changer !!)
    while (!isFin) {
        ///MENU DU JEU
        while (!isFin) {
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
                al_flip_display();
                al_clear_to_color(black);
                draw = 0;
            }
        }

        ///JOUER
        while (!isFin) {
            al_wait_for_event(queue, &event);
            switch (event.type) {
                case ALLEGRO_EVENT_KEY_DOWN : {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_ESCAPE : {
                            isFin = 1;
                            break;
                        }
                        case ALLEGRO_KEY_Z : {
                            break;
                        }
                        case ALLEGRO_KEY_S : {
                            break;
                        }
                        case ALLEGRO_KEY_D : {
                            break;
                        }
                        case ALLEGRO_KEY_Q: {
                            break;
                        }
                    }
                    break;
                }
                case ALLEGRO_EVENT_KEY_UP : {
                    for(int i = 0 ; i < 4 ; i++) {

                    }
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

                al_flip_display();
                al_clear_to_color(black);
                draw = 0;
            }
        }
    }
    return 0;
}