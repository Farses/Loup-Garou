
#include "font.h"


void load_responsive_fonts(int screen_width, int screen_height, ALLEGRO_FONT **title_font,
                           ALLEGRO_FONT **subtitle_font, ALLEGRO_FONT **text_font,
                           ALLEGRO_FONT **small_font) {
    // Définir des facteurs d'échelle basés sur la résolution
    // Ces facteurs permettent d'adapter les polices en fonction de la taille de l'écran
    float scale_factor = screen_width / 1920.0; // 1920 est une résolution de référence

    // Définir les tailles des polices proportionnellement à la taille de l'écran
    int title_size = (int)(100 * scale_factor);      // Grande police pour les titres
    int subtitle_size = (int)(72 * scale_factor);    // Police moyenne pour les sous-titres
    int text_size = (int)(50 * scale_factor);        // Police normale pour le texte courant
    int small_size = (int)(30 * scale_factor);       // Petite police pour les détails

    // Assurer une taille minimale pour chaque police
    title_size = title_size < 40 ? 40 : title_size;
    subtitle_size = subtitle_size < 30 ? 30 : subtitle_size;
    text_size = text_size < 20 ? 20 : text_size;
    small_size = small_size < 12 ? 12 : small_size;

    // Charger les polices avec les tailles adaptées
    *title_font = al_load_ttf_font("../Font/Nintendo.ttf", title_size, 0);
    *subtitle_font = al_load_ttf_font("../Font/Nintendo.ttf", subtitle_size, 0);
    *text_font = al_load_ttf_font("../Font/Nintendo.ttf", text_size, 0);
    *small_font = al_load_ttf_font("../Font/Nintendo.ttf", small_size, 0);

    // Vérifier si les polices ont été chargées correctement
    if (!*title_font || !*subtitle_font || !*text_font || !*small_font) {
        fprintf(stderr, "Erreur: Impossible de charger les polices.\n");
        // Gérer l'erreur selon vos besoins (exit, fallback, etc.)
    }
}

void handle_resize_event(ALLEGRO_EVENT *event, ALLEGRO_FONT **title_font,
                         ALLEGRO_FONT **subtitle_font, ALLEGRO_FONT **text_font,
                         ALLEGRO_FONT **small_font) {
    // Obtenir les nouvelles dimensions
    int new_width = event->display.width;
    int new_height = event->display.height;

    // Confirmer le redimensionnement
    al_acknowledge_resize(event->display.source);

    // Libérer les anciennes polices
    al_destroy_font(*title_font);
    al_destroy_font(*subtitle_font);
    al_destroy_font(*text_font);
    al_destroy_font(*small_font);

    // Charger les nouvelles polices adaptées à la nouvelle résolution
    load_responsive_fonts(new_width, new_height, title_font, subtitle_font, text_font, small_font);
}
