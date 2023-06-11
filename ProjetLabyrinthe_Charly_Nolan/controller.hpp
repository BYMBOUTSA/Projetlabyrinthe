// fichier : controller.hpp
#pragma once

#include "vue.hpp"
#include "modele.hpp"
#include <gtkmm.h>

class Controller
{
private:
    Labyrinth laby;
    Vue vue;
    Gtk::Window dialog_window;  // Fenêtre contextuelle affichant le message de victoire
    Gtk::Label dialog_label;
    Gtk::Button dialog_button;

public:
    Controller()
    {
        // Création du contrôleur

        // Création du labyrinthe initial avec une taille de 2x2
        laby = Labyrinth(2, 2);

        // Configuration de la position de départ et de l'arrivée dans la vue
        vue.get_grid().set_start_position(0, 0);
        vue.get_grid().set_exit_position(2, 2);


        // Configuration de la fenêtre contextuelle
        dialog_button.signal_clicked().connect(sigc::mem_fun(*this, &Controller::on_dialog_button_clicked));
        dialog_window.set_border_width(10);
        dialog_window.set_position(Gtk::WIN_POS_CENTER);
        dialog_window.set_default_size(400, 300);
        dialog_window.set_resizable(true);
        dialog_window.set_title("Victoire !");
        dialog_window.add(dialog_label);
        dialog_label.set_text("Vous ếtes à la sortie ! Alors félicitation, vous avez gagné !");
        dialog_window.show_all();
        dialog_window.hide();

        // Liaison des signaux et des événements

        // Gestion de l'événement de changement de taille dans la vue
        vue.link_new_size([&]() 
        {
            int height = vue.get_laby_height();
            int width = vue.get_laby_width();
            // Assurer une taille minimale de 2x2 pour le labyrinthe
            if (height <= 1) { height = 2; }
            if (width <= 1) { width = 2; }

            // Création d'un nouveau labyrinthe et mise à jour de la vue
            laby = Labyrinth(height, width);
            vue.new_grid(height, width);
            vue.get_grid().reset_all();

            // Réinitialisation de la position du joueur
            vue.get_grid().set_start_position(0, 0);
            
            // Position de l'arrivée en bas à droite
            vue.get_grid().set_exit_position(width - 1, height - 1);

            vue.queue_draw();
        });

        // Gestion de l'événement du bouton de fusion dans la vue
        vue.link_fusion([&]() 
        {
            vue.get_grid().reset_all();
            laby.reset_all();
            laby.fusion();
            vue.get_grid().set_door( translate_door_position() );
            vue.queue_draw();
        });

        // Gestion de l'événement du bouton Aldous-Broder dans la vue
        vue.link_aldous([&]() 
        {
            vue.get_grid().reset_all();
            laby.reset_all();
            laby.construire_aldous_broder();
            vue.get_grid().set_door( translate_door_position() );
            vue.queue_draw();
        });

        // Gestion des événements des touches de direction dans la vue
        vue.get_grid().link_arrow_key([&](GdkEventKey* event) -> bool
        {
            // Réduire le code de la touche dans la plage [0-3]
            int side = (event->keyval - 1) & 3;

            if (event->keyval >= GDK_KEY_Left && event->keyval <= GDK_KEY_Down)
            {
                // La touche enfoncée est une touche de direction
                int movement_done = laby.move_position(side);
                if (movement_done == 0)
                {
                    // Ne rien faire
                    return true;
                }
                // Mettre à jour la position du joueur dans la vue
                Coordinate player_pos = laby.get_position();
                vue.get_grid().set_location(player_pos.first, player_pos.second);

                if (movement_done == 2)
                {
                    check_player_reached_final(); // Afficher la fenêtre contextuelle lorsque le joueur atteint la case verte
                }
            }
            vue.queue_draw();
            return true;
        });

        vue.link_resolver([&]()
        {
            vue.get_grid().set_solution( laby.resolve_labyrinthe() );
            vue.queue_draw();
        });
        
        vue.link_move_auto([&]()
        {
            vue.get_grid().on_move_auto_clicked();
        });



        vue.link_move_auto_stop([&]()
        {
           vue.get_grid().on_move_auto_stop_clicked(); 
        });
    }

    // Accesseur pour la vue 
    Vue& get_vue() { return vue; }


    // Création de la carte des portes dans la vue
    const std::map<Coordinate, std::pair<bool, bool>> translate_door_position() const
    {
        std::map<Coordinate, std::pair<bool, bool>> door_map;
        for (const auto &pair: laby)
        {
            Coordinate current_coord{pair.first.first, pair.first.second};
            Coordinate neighbour_down{current_coord.first, current_coord.second+1};
            Coordinate neighbour_right{current_coord.first+1, current_coord.second};
            
            bool door_down = laby.is_neighbour(current_coord, neighbour_down);
            bool door_right = laby.is_neighbour(current_coord, neighbour_right);
            door_map.insert( {current_coord, {door_down, door_right}} );
        }
        return door_map;
    }


    // Vérifier si le joueur a atteint la position finale et afficher la fenêtre contextuelle
    void check_player_reached_final()
    {
        int location_x = vue.get_grid().get_location_x();
        int location_y = vue.get_grid().get_location_y();
        int exit_x = vue.get_grid().get_exit_x();
        int exit_y = vue.get_grid().get_exit_y();

        if (location_x == exit_x && location_y == exit_y)
        {
            dialog_window.show_all();
        }
    }

    // Gérer l'événement de clic sur le bouton dans la fenêtre contextuelle
    void on_dialog_button_clicked() {
        dialog_window.hide();
    }
};
