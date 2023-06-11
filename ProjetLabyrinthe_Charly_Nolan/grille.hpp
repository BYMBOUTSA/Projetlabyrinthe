#pragma once

#include <gtkmm.h>
#include <map>
#include <iostream>
#include <utility>
#include <functional>

class Grille : public Gtk::DrawingArea {
private:
    int cote_case = 80;
    int nb_ligne;
    int nb_colonne;
    int location_x = 0;
    int location_y = 0;
    int start_x = 0;
    int start_y = 0;
    int exit_x = 0;
    int exit_y = 0;
    const int timeout_value = 1000;
    bool automatic_move = false;
    int step_auto = 0;

    std::map<std::pair<int, int>, std::pair<bool, bool>> door_map;
    std::vector<std::pair<int, int>> solution;

public:
    Grille(const int colonne, const int ligne) : nb_ligne(ligne), nb_colonne(colonne) {
        add_events(Gdk::BUTTON_PRESS_MASK);
        add_events(Gdk::KEY_PRESS_MASK);
        
        this->signal_draw().connect(sigc::mem_fun(*this, &Grille::on_draw));
        this->set_size_request(cote_case * colonne, cote_case * ligne);

        sigc::connection unTimer = Glib::signal_timeout().connect(
            sigc::mem_fun(*this , &Grille::on_timeout) , this->timeout_value);
    }

    virtual ~Grille() {}

    void set_row(const int l) { nb_ligne = l; }
    void set_column(const int c) { nb_colonne = c; }

    void link_arrow_key(std::function<bool(GdkEventKey* event)> f);
    

    void set_location(const int x, const int y);

    void move_location(const int dx, const int dy) {
       location_x += dx;
       location_y += dy;
       queue_draw();
    }

    void set_solution(std::vector<std::pair<int, int>> soluce)
    {
        solution = soluce;
    }

    void set_door(std::map<std::pair<int, int>, std::pair<bool, bool>> m)
    {
        door_map = m;
    }

    void set_start_position(int x, int y) {
        start_x = x;
        start_y = y;
        location_x = x;
        location_y = y;
    }

    void set_exit_position(int x, int y) {
        exit_x = x;
        exit_y = y;
    }

    int get_start_x() const {
        return start_x;
    }

    int get_start_y() const {
        return start_y;
    }

    int get_exit_x() const {
        return exit_x;
    }

    int get_exit_y() const {
        return exit_y;
    }

    int get_location_x() const {
        return location_x;
    }

    int get_location_y() const {
        return location_y;
    }

    int get_column_count() const {
        return nb_colonne;
    }

    int get_row_count() const {
        return nb_ligne;
    }

    void set_case_size(int size) {
        cote_case = size;
    }

    void reset_all();

    void on_move_auto_clicked();
    void on_move_auto_stop_clicked();


    /**
     * Action a réaliser tous les timeout_value, i.e. les deplacement automatique
     */
    bool on_timeout();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        int current_x = 0, current_y = 0;
        int allocation_width = get_allocated_width();
        int allocation_height = get_allocated_height();
        cote_case = std::min(allocation_width / nb_colonne, allocation_height / nb_ligne);
        cr->rectangle(0, 0, allocation_width, allocation_height);
        cr->fill();
        cr->set_source_rgb(1, 1, 1);

        for (int colonne = 0; colonne <= nb_colonne; colonne++) {
            cr->move_to(current_x, 0);
            cr->line_to(current_x, cote_case * nb_ligne);
            cr->stroke();
            current_x += cote_case;
        }

        for (int ligne = 0; ligne <= nb_ligne; ligne++) {
            cr->move_to(0, current_y);
            cr->line_to(cote_case * nb_colonne, current_y);
            cr->stroke();
            current_y += cote_case;
        }

        // Dessine du noir sur les murs qui sont enlevés
        cr->set_source_rgb(0, 0, 0);
        for (const auto& pair : door_map) {
            int current_x = pair.first.first;
            int current_y = pair.first.second;

            if (pair.second.first) {
                // Door down
                cr->move_to(current_x * cote_case, (current_y + 1) * cote_case);
                cr->line_to((current_x + 1) * cote_case, (current_y + 1) * cote_case);
                cr->stroke();
            }
            if (pair.second.second) {
                // Door right
                cr->move_to((current_x + 1) * cote_case, current_y * cote_case);
                cr->line_to((current_x + 1) * cote_case, (current_y + 1) * cote_case);
                cr->stroke();
            }
        }

        // dessine la solution
        if (!solution.empty()) {
            cr->set_source_rgb(0.0, 0.0, 1.0);
            current_x = solution[0].first;
            current_y = solution[0].second;
            for (const auto& coord : solution) {
                int next_x = coord.first;
                int next_y = coord.second;

                cr->move_to(current_x * cote_case + cote_case / 2, current_y * cote_case + cote_case / 2);
                cr->line_to(next_x * cote_case + cote_case / 2, next_y * cote_case + cote_case / 2);
                cr->stroke();

                current_x = next_x;
                current_y = next_y;
            }
        }

        // Dessiner la position initiale (en rouge)
        cr->set_source_rgb(1.0, 0.0, 0.0);
        cr->rectangle((start_x + 0.05) * cote_case, (start_y + 0.05) * cote_case, 0.9 * cote_case, 0.9 * cote_case);
        cr->fill();

        // Dessiner la position de sortie (en vert)
        cr->set_source_rgb(0.0, 1.0, 0.0);
        cr->rectangle((exit_x + 0.05) * cote_case, (exit_y + 0.05) * cote_case, 0.9 * cote_case, 0.9 * cote_case);
        cr->fill();

        cr->set_source_rgb(1, 1, 1);
        // draw player position
        cr->arc((location_x) * cote_case + cote_case / 2, (location_y) * cote_case + cote_case / 2, cote_case / 3, 0, 2 * M_PI);
        cr->stroke();

        return true;
    }

    
};
