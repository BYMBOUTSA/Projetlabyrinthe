// fichier : vue.cpp
#include "vue.hpp"

// Associe une fonction à exécuter lorsqu'une nouvelle taille est spécifiée
void Vue::link_new_size(std::function<void()> f) {
  e_width.signal_activate().connect(
      f); // Connecte le signal "activate" du champ de saisie e_width à la
          // fonction f
  e_height.signal_activate().connect(
      f); // Connecte le signal "activate" du champ de saisie e_height à la
          // fonction f
  b_restart.signal_clicked().connect(
      f); // Connecte le signal "clicked" du bouton b_restart à la fonction f
}

// Associe une fonction à exécuter lorsqu'on clique sur le bouton b_fusion
void Vue::link_fusion(std::function<void()> f) {
  b_fusion.signal_clicked().connect(
      f); // Connecte le signal "clicked" du bouton b_fusion à la fonction f
}

// Associe une fonction à exécuter lorsqu'on clique sur le bouton b_aldous
void Vue::link_aldous(std::function<void()> f) {
  b_aldous.signal_clicked().connect(
      f); // Connecte le signal "clicked" du bouton b_aldous à la fonction f
}

// Obtient la largeur du labyrinthe à partir du champ de saisie e_width
int Vue::get_laby_width() const {
  auto buffer = e_width.get_buffer(); // Récupère le buffer associé au champ de
                                      // saisie e_width
  return atoi(
      (buffer->get_text()
           .c_str())); // Convertit le texte du buffer en entier et le renvoie
}

// Obtient la hauteur du labyrinthe à partir du champ de saisie e_height
int Vue::get_laby_height() const {
  auto buffer = e_height.get_buffer(); // Récupère le buffer associé au champ de
                                       // saisie e_height
  return atoi(
      (buffer->get_text()
           .c_str())); // Convertit le texte du buffer en entier et le renvoie
}
