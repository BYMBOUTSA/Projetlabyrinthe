#include "grille.hpp"

void Grille::link_arrow_key(std::function<bool(GdkEventKey *event)> f) {
  signal_key_press_event().connect(f, false);
}

void Grille::set_location(const int x, const int y) {
  location_x = x; // Définir la position x du joueur
  location_y = y; // Définir la position y du joueur
  queue_draw();   // Mettre à jour l'affichage de la grille
}
