#ifndef AAAAAAAAAA_FONT_H
#define AAAAAAAAAA_FONT_H

#include "game.h"


void handle_resize_event(ALLEGRO_EVENT *event, ALLEGRO_FONT **title_font,
                         ALLEGRO_FONT **subtitle_font, ALLEGRO_FONT **text_font,
                         ALLEGRO_FONT **small_font) ;

void load_responsive_fonts(int screen_width, int screen_height, ALLEGRO_FONT **title_font,
                      ALLEGRO_FONT **subtitle_font, ALLEGRO_FONT **text_font,
                      ALLEGRO_FONT **small_font);


#endif //AAAAAAAAAA_FONT_H
