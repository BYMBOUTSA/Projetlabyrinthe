#pragma once

#include <utility>
#include <set>
#include <map>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "observable.hpp"

using Coordinate = std::pair<int, int>;

class Cell
{
private:
    Coordinate coordinate;
	int id;
    std::set<Coordinate> adgency_list;

public:
    Cell(const Coordinate &coo, const int id=0): coordinate{coo}, id{id}, adgency_list{} 
	{
		std::srand(std::time(nullptr));
	}

    /**
     * Renvoie les coordonnées de l'objet courant
    */
    const Coordinate &get_coordinate() const;


	/**
 	 * Renvoie l'identifiant de la cellule
	*/
	int get_id() const;


	/**
 	 * Change la valeur de l'identification de la cellule courante
   	*/
	void set_id(const int new_id);


    /**
     * Ajoute les coordonnées d'une autre cellule a la liste d'adjacence de cet objet
     * La cellule est donc une voisine (pas de mur) avec l'objet courant
    */
    bool add_neighbour(const Cell &neigh);


    /**
     * Retire tous les voisins de la cellule courante
    */
    void remove_neighbour();


    /**
     * Permet d'utiliser un for each sur l'objet courant en parcourant tous les voisins
    */
    auto begin() const {return adgency_list.begin();}
	auto begin() {return adgency_list.begin();}

    /**
     * Permet d'utiliser un for each sur l'objet courant en parcourant tous les voisins
    */
    auto end() const {return adgency_list.end();}
	auto end() {return adgency_list.end();}


    /**
     * Permet de rapidement afficher le contenu du labyrinthe sur la sortie standard
    */
    friend std::ostream &operator<<(std::ostream &f, const Cell &c)
    {
        auto coo = c.get_coordinate();
        f << "(" << coo.first << "," << coo.second << "): [ ";
        for (const auto &coo_i: c)
        {
            f << "(" << coo_i.first << "," << coo_i.second << ") ";
        }
        return f << "]" << " id = " << c.get_id();
    }
};

class Labyrinth: public Observable<Labyrinth>
{
private:
    int height;
    int width;
    std::map<Coordinate, Cell> graph;

public:
    /**
     * Construit un labyrinthe de taille height * width avec des cellules vierges 
    */
    Labyrinth(int height, int width): height{height}, width{width} 
    {
        for (int i=0; i<height; i++)
        {
            for (int j=0; j<width; j++)
            {
                Coordinate coo({i, j});
                graph.insert({coo, Cell(coo, i*width+j)});
            }
        }
    }

	/**
 	 * Vérifie si la coordonnée passé en paramètre est inclus dans le labyrinthe
   	*/
	bool is_in_graph(const Coordinate & coord) const;


    /**
     * Pour chacune des cellules données, ajoute l'autre en voisine
     * Le mur entre les deux cellules est donc enlevé 
    */
    void remove_wall(Cell &c1, Cell &c2);


    /**
     * Pour chacune des coordonnées, récupère les 2 cellules correspondantes dans le graph, ajoute l'autre en voisine
     * Le mur entre les deux cellules est donc enlevé
    */    
    void remove_wall(const Coordinate &c1, const Coordinate &c2);


    /**
     * pour chaque cellule du labytinthe, retire toutes ses voisine
     * A pour effet de remettre en place tous les murs du labyrinthe
    */
    void reset_wall();


    /**
     * Permet d'utiliser un énoncé for each pour parcourir toutes les cellules
    */
    auto begin() const {return graph.begin();}
    

    /**
     * Permet d'utiliser un énoncé for each pour parcourir toutes les cellules
    */
    auto end() const {return graph.end();}


    /**
     * Accède à la cellule de coordonnée "c"
    */
	Cell &get_cell(const Coordinate &c);

	const Cell &get_cell(const Coordinate &c) const;

    //Cell &operator()(const Coordinate &c);
	//const Cell &operator()(const Coordinate &c) const;

	/**
 	 * Vérifie si toutes les cellules ont le meme identifiant "id"
	 * Renvoie true si c'est le cas, false sinon
	*/
	bool is_fusion_finished() const;


	/**
 	 * Met en place l'algorithme de construction de mur Fusion
	 * Modifie le labyrinthe actuel
	*/
	void fusion();


    friend std::ostream &operator<<(std::ostream &f, const Labyrinth &l)
    {
        for (const auto &cell: l)
        {
            f << cell.second << "\n";
        }
        return f;
    }
};