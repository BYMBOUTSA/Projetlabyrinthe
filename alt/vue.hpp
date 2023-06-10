// fichier : vue.hpp
#pragma once
#include <gtkmm.h>
#include <vector>
#include <string>
#include <functional>

#include "grille.hpp"

// Classe représentant la vue principale de l'application
class Vue: public Gtk::Window{
private:
    Gtk::Grid main_grid; // Conteneur principal pour organiser les éléments de la fenêtre
    Grille draw; // Objet représentant la grille
    Gtk::Entry e_width; // Champ de saisie pour la largeur
    Gtk::Entry e_height; // Champ de saisie pour la hauteur
    Gtk::Label l_width; // Étiquette pour afficher "Largeur:"
    Gtk::Label l_height; // Étiquette pour afficher "Hauteur:"
    Gtk::Button b_restart; // Bouton pour redémarrer
    Gtk::Button b_close; // Bouton pour fermer la fenêtre
    Gtk::Button b_fusion; // Bouton pour l'algorithme de fusion
    Gtk::Button b_aldous; // Bouton pour l'algorithme d'Aldous Broder
public:
    Vue() : main_grid(), draw(3, 3), e_width{}, e_height{}, l_width{"Largeur:"}, l_height{"Hauteur:"},
	b_restart("Restart"), b_close("Close"), b_fusion("Fusion"), b_aldous("Aldous Broder")
    {
		resize(300, 775); // Définit la taille de la fenêtre

        add_events(Gdk::KEY_PRESS_MASK); // Ajoute les événements de pression de touche à la fenêtre

		draw.set_hexpand(true); // Définit l'extension horizontale de l'objet draw dans la grille
		draw.set_vexpand(true); // Définit l'extension verticale de l'objet draw dans la grille
		main_grid.set_column_homogeneous(true); // Définit que les colonnes de la grille ont une largeur uniforme

        b_close.signal_clicked().connect([&]() { Gtk::Window::close(); }); // Connecte le signal "clicked" du bouton b_close à la fermeture de la fenêtre

        main_grid.attach(draw, 0, 0, 4, 1); // Attache l'objet draw à la grille aux positions (0,0) avec une largeur de 4 colonnes et une hauteur de 1 ligne

        main_grid.attach(l_width, 0, 1); // Attache l'étiquette l_width à la grille à la position (0,1)
        main_grid.attach(e_width, 1, 1); // Attache le champ de saisie e_width à la grille à la position (1,1)
        main_grid.attach(l_height, 2, 1); // Attache l'étiquette l_height à la grille à la position (2,1)
        main_grid.attach(e_height, 3, 1); // Attache le champ de saisie e_height à la grille à la position (3,1)
        
		main_grid.attach(b_fusion, 1, 2); // Attache le bouton b_fusion à la grille à la position (1,2)
		main_grid.attach(b_aldous, 2, 2); // Attache le bouton b_aldous à la grille à la position (2,2)

        main_grid.attach(b_restart, 1, 3); // Attache le bouton b_restart à la grille à la position (1,3)
        main_grid.attach(b_close, 2, 3); // Attache le bouton b_close à la grille à la position (2,3)
        add(main_grid); // Ajoute la grille principale à la fenêtre

        draw.set_can_focus(true); // Permet à l'objet draw de recevoir le focus
        draw.set_focus_on_click(true); // Définit que l'objet draw reçoit le focus lors d'un clic
        show_all_children(); // Affiche tous les éléments enfants de la fenêtre
    }

    /**
     * Associe la fonction f aux widgets liés à la taille (e_width, e_height et b_restart)
    */
	void link_new_size(std::function<void()> f);

	/**
	 * Associe la fonction f au bouton "Fusion"
	*/
	void link_fusion(std::function<void()> f);

	/**
	 * Associe la fonction f au bouton "Aldous Broder"
	*/
	void link_aldous(std::function<void()> f);

    /**
	 * Récupère la valeur du champ de saisie e_width (la largeur du labyrinthe)
    */
	int get_laby_width() const;

    /**
	 * Récupère la valeur du champ de saisie e_height (la hauteur du labyrinthe)
    */
	int get_laby_height() const;

	/**
	 * Récupère la référence vers l'objet draw (la grille)
	*/
	Grille &get_grid() { return draw; }

	/**
	 * Crée une nouvelle grille avec le nombre de lignes r et de colonnes c
	*/
	void new_grid(const int r, const int c);
};

