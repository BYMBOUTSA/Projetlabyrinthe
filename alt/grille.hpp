#pragma once

#include <gtkmm.h>
#include <map>
#include <iostream>
#include <utility>
#include <functional>

class Grille : public Gtk::DrawingArea {
private:
    const int timeout_value = 1000;
    int cote_case = 80;  // Taille d'un côté de chaque case
    int nb_ligne;  // Nombre de lignes dans la grille
    int nb_colonne;  // Nombre de colonnes dans la grille
    int location_x = 0;  // Position x du joueur dans la grille
    int location_y = 0;  // Position y du joueur dans la grille
    int start_x = 0;  // Position x de départ dans la grille
    int start_y = 0;  // Position y de départ dans la grille
    int exit_x = 0;  // Position x de sortie dans la grille
    int exit_y = 0;  // Position y de sortie dans la grille

    std::multimap<int, int> vertical_door;  // Carte des portes verticales
    std::multimap<int, int> horizontal_door;  // Carte des portes horizontales

public:
    Grille(const int colonne, const int ligne) : nb_ligne(ligne), nb_colonne(colonne) {
        add_events(Gdk::BUTTON_PRESS_MASK);
        add_events(Gdk::KEY_PRESS_MASK);
        
        this->signal_draw().connect(sigc::mem_fun(*this, &Grille::on_draw));
        this->set_size_request(cote_case * colonne, cote_case * ligne);
    }

    virtual ~Grille() {}

    void set_row(const int l) { nb_ligne = l; }  // Définir le nombre de lignes dans la grille
    void set_column(const int c) { nb_colonne = c; }  // Définir le nombre de colonnes dans la grille

    void link_arrow_key(std::function<bool(GdkEventKey* event)> f);  // Lier les touches de direction à une fonction

    void set_location(const int x, const int y);  // Définir la position du joueur dans la grille

    void move_location(const int dx, const int dy) {
       location_x += dx;
       location_y += dy;
       queue_draw();
    }

    void set_door(const std::multimap<int, int>& v_d, const std::multimap<int, int>& h_d) {
        vertical_door = v_d;  // Définir les portes verticales dans la grille
        horizontal_door = h_d;  // Définir les portes horizontales dans la grille
    }

    void set_start_position(int x, int y) {
        start_x = x;  // Définir la position de départ x dans la grille
        start_y = y;  // Définir la position de départ y dans la grille
        location_x = x;  // Définir la position x du joueur au départ
        location_y = y;  // Définir la position y du joueur au départ
    }

    void set_exit_position(int x, int y) {
        exit_x = x;  // Définir la position de sortie x dans la grille
        exit_y = y;  // Définir la position de sortie y dans la grille
    }


    int get_start_x() const {
        return start_x;  // Obtenir la position de départ x dans la grille
    }

    int get_start_y() const {
        return start_y;  // Obtenir la position de départ y dans la grille
    }

    int get_exit_x() const {
        return exit_x;  // Obtenir la position de sortie x dans la grille
    }

    int get_exit_y() const {
        return exit_y;  // Obtenir la position de sortie y dans la grille
    }

    int get_location_x() const {
        return location_x;  // Obtenir la position x actuelle du joueur dans la grille
    }

    int get_location_y() const {
        return location_y;  // Obtenir la position y actuelle du joueur dans la grille
    }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        int current_x = 0, current_y = 0;
        int allocation_width = get_width();  // Largeur allouée à la grille
        int allocation_height = get_height();  // Hauteur allouée à la grille
        cote_case = std::min(allocation_width / nb_colonne, allocation_height / nb_ligne);  // Calcul de la taille des cases
        cr->rectangle(0, 0, allocation_width, allocation_height);
        cr->fill();
        cr->set_source_rgb(1, 1, 1);  // Couleur de fond de la grille

        // Dessiner les lignes verticales
        for (int colonne = 0; colonne <= nb_colonne; colonne++) {
            cr->move_to(current_x, 0);
            cr->line_to(current_x, cote_case * nb_ligne);
            cr->stroke();
            current_x += cote_case;
        }

        // Dessiner les lignes horizontales
        for (int ligne = 0; ligne <= nb_ligne; ligne++) {
            cr->move_to(0, current_y);
            cr->line_to(cote_case * nb_colonne, current_y);
            cr->stroke();
            current_y += cote_case;
        }

        cr->set_source_rgb(0, 0, 0);  // Couleur des murs

        // Dessiner les murs horizontaux
        for (auto pair: horizontal_door)
        {
            int current_row = pair.first;
            int current_column = pair.second;
            cr->move_to(current_column*cote_case, (current_row+1)*cote_case);
            cr->line_to((current_column+1)*cote_case, (current_row+1)*cote_case);
            cr->stroke();
        }

        // Dessiner les murs verticaux
        for (auto pair: vertical_door)
        {
            int current_row = pair.first;
            int current_column = pair.second;
            
            cr->move_to((current_column+1)*cote_case, current_row*cote_case);
            cr->line_to((current_column+1)*cote_case, (current_row+1)*cote_case);
            cr->stroke();
        }

        cr->set_source_rgb(1, 1, 1);  // Couleur du joueur

        // Dessiner la position initiale (en rouge)
        cr->set_source_rgb(1.0, 0.0, 0.0);
        cr->rectangle((start_x+0.05) * cote_case, (start_y+0.05) * cote_case, 0.9*cote_case, 0.9*cote_case);
        cr->fill();

        // Dessiner la position de sortie (en vert)
        cr->set_source_rgb(0.0, 1.0, 0.0);
        cr->rectangle((exit_x+0.05) * cote_case, (exit_y+0.05) * cote_case, 0.9*cote_case, 0.9*cote_case);
        cr->fill();

        cr->set_source_rgb(1, 1, 1);

        // Dessiner la position du joueur
        cr->arc((location_x) * cote_case + cote_case / 2, (location_y) * cote_case + cote_case / 2, cote_case / 3, 0, 2 * M_PI);
        cr->stroke();

        return true;
    }
};
