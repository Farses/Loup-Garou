#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "client.h"
#include  "server.h"
#include  "font.h"



#pragma comment(lib, "ws2_32.lib")

void init(Jeu* game, float width, float height){
    game->width = width ;
    game->height = height ;
    game->modeJeu=0 ;
    game->mouseIsPressed = false ;
    game->mouse_x= 0 ;
    game->mouse_y= 0 ;
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
        while (jeu.modeJeu == 0) {

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