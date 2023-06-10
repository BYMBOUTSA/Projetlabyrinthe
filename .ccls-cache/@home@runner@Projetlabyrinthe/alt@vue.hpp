// fichier : vue.hpp
#pragma once
#include <gtkmm.h>
#include <vector>
#include <string>
#include <functional>

#include "grille.hpp"
#include "modele.hpp"


class Vue: public Gtk::Window{
private:
    Gtk::Grid main_grid;
    Grille draw;
    Gtk::Entry e_width;
    Gtk::Entry e_height;
    Gtk::Label l_width;
    Gtk::Label l_height;
    Gtk::Button b_restart;
    Gtk::Button b_close;
	Gtk::Button b_fusion;
	Gtk::Button b_aldous;
public:
    Vue() : main_grid(), draw(3, 3), e_width{}, e_height{}, l_width{"Largeur:"}, l_height{"Hauteur:"}, 
	b_restart("Restart"), b_close("Close"), b_fusion("Fusion"), b_aldous("Aldous Broder")
    {
		resize(300, 775);

        add_events(Gdk::KEY_PRESS_MASK);
		
        //get_grid().signal_key_press_event().connect([&](GdkEventKey *event)->bool {get_grid().move_location(1, 0); queue_draw(); std::cout <<"ui\n";return true;});


		draw.set_hexpand(true);
		draw.set_vexpand(true);
		main_grid.set_column_homogeneous(true);

        b_close.signal_clicked().connect([&]() { Gtk::Window::close(); });

        main_grid.attach(draw, 0, 0, 4, 1);

        main_grid.attach(l_width, 0, 1);
        main_grid.attach(e_width, 1, 1);
        main_grid.attach(l_height, 2, 1);
        main_grid.attach(e_height, 3, 1);
        
		main_grid.attach(b_fusion, 1, 2);
		main_grid.attach(b_aldous, 2, 2);

        main_grid.attach(b_restart, 1, 3);
        main_grid.attach(b_close, 2, 3);
        add(main_grid);

        draw.set_can_focus(true);
        draw.set_focus_on_click(true);
        show_all_children();
    }

    /**
     * Link the 3 size related widget to the given function
	 * The widgets are: e_width, e_height and b_restart
    */
	void link_new_size(std::function<void()> f);


	/**
	 * Link the button "Fusion" to the given function
	*/
	void link_fusion(std::function<void()> f);


	/**
	 * Link the button "Aldous Broder" to the given function
	*/
	void link_aldous(std::function<void()> f);


    /**
	 * Access the content of the width entry
    */
	int get_laby_width() const;


    /**
	 * Access the content of the height entry
    */
	int get_laby_height() const;

	Grille &get_grid() {return draw;}

	void new_grid(const int r, const int c)
	{
		draw.set_row(r);
		draw.set_column(c);
	}
	
};



