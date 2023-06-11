// fichier : grille.cpp
#include "grille.hpp"


void Grille::link_arrow_key(std::function<bool(GdkEventKey* event)> f)
{
    signal_key_press_event().connect(f, false);
}

void Grille::set_location(const int x, const int y)
{
    location_x = x;
    location_y = y;
    queue_draw();
}

void Grille::reset_all()
{
    solution.clear();
    door_map.clear();
    step_auto = 0;
    automatic_move = false;
}

void Grille::on_move_auto_clicked() {
    automatic_move = true;
}

void Grille::on_move_auto_stop_clicked() {
    automatic_move = false;
}


bool Grille::on_timeout()
{
    if (automatic_move && !solution.empty())
    {
        std::pair<int, int> next_step = solution[step_auto];
        set_location(next_step.first, next_step.second); 
        step_auto++;
        queue_draw();
    }
    if ((long unsigned int)step_auto == solution.size())
    {
        step_auto = 0;
        automatic_move = false;
    }
    return true;
}


