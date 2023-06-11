// fichier : modele.hpp
#pragma once

#include <utility>
#include <set>
#include <map>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "observable.hpp"
#include "position.hpp"

using Coordinate = std::pair<int, int>;

class Cell
{
private:
    Coordinate coordinate; // Coordonnées de la cellule
	int id; // Identifiant de la cellule
    std::set<Coordinate> adgency_list; // Liste des cellules voisines

public:
    Cell(const Coordinate &coo, const int id=0): coordinate{coo}, id{id}, adgency_list{} {}

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
     * Ajoute les coordonnées d'une autre cellule à la liste d'adjacence de cet objet.
     * La cellule est donc une voisine (pas de mur) de l'objet courant.
    */
    bool add_neighbour(const Cell &neigh);

    /**
     * Retire tous les voisins de la cellule courante.
    */
    void remove_neighbour();

    /**
     * Vérifie si la coordonnée donnée est dans la liste d'adjacence de la cellule courante.
    */
    bool is_neighbour(const Coordinate &coord) const;

    /**
     * Permet d'utiliser une boucle for-each sur l'objet courant pour parcourir tous les voisins.
    */
    auto begin() const {return adgency_list.begin();}
	auto begin() {return adgency_list.begin();}

    /**
     * Permet d'utiliser une boucle for-each sur l'objet courant pour parcourir tous les voisins.
    */
    auto end() const {return adgency_list.end();}
	auto end() {return adgency_list.end();}

    /**
     * Permet d'afficher rapidement le contenu de la cellule sur la sortie standard.
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
    int height; // Hauteur du labyrinthe
    int width; // Largeur du labyrinthe
    Position currentPosition; // Position actuelle du joueur
    Coordinate wining_coord; // Coordonnées de la case gagnante
    std::map<Coordinate, Cell> graph; // Représentation du labyrinthe sous forme de graphe

public:
    /**
     * Construit un labyrinthe de taille height * width avec des cellules vierges.
    */
    Labyrinth(int height=3, int width=3): height{height}, width{width}, currentPosition(0, 0)
    {
        wining_coord = {width-1, height-1};

        for (int j=0; j<height; j++)
        {
            for (int i=0; i<width; i++)
            {
                Coordinate coo({i, j});
                graph.insert({coo, Cell(coo, j*width+i)});
            }
        }
    }

    /**
     * Demande au modèle de déplacer la position du joueur dans une direction.
     * Le modèle vérifie si le déplacement est possible et l'effectue si c'est le cas.
     * Renvoie :
     *   - 0 si le déplacement est refusé
     *   - 1 si le déplacement est accepté
     *   - 2 si le déplacement est accepté et que le joueur gagne
    */
    int move_position(const int side);

    /**
     * Vérifie si les deux coordonnées passées sont voisines.
     * Teste si la cellule en "coord2" est dans la liste d'adjacence de la cellule en "coord1".
    */
    bool is_neighbour(const Coordinate &coord1, const Coordinate &coord2) const;
    
    /**
     * Vérifie si la coordonnée passée en paramètre correspond à la case gagnante.
    */
    bool is_winning(const Coordinate &coord) const;

	/**
 	 * Vérifie si la coordonnée passée en paramètre est incluse dans le labyrinthe.
   	*/
	bool is_in_graph(const Coordinate & coord) const;

    /**
     * Pour chacune des cellules données, ajoute l'autre en tant que voisine.
     * Le mur entre les deux cellules est donc enlevé.
    */
    void remove_wall(Cell &c1, Cell &c2);

    /**
     * Pour chacune des coordonnées, récupère les 2 cellules correspondantes dans le graphe, ajoute l'autre en tant que voisine.
     * Le mur entre les deux cellules est donc enlevé.
    */    
    void remove_wall(const Coordinate &c1, const Coordinate &c2);

    /**
     * Pour chaque cellule du labyrinthe, retire tous ses voisins.
     * Cela a pour effet de remettre en place tous les murs du labyrinthe.
    */
    void reset_wall();


    /**
     * Renitialise la position du joueur
     */
    void reset_position();

    /**
     * Renitialise les murs et la position du joueur pour pouvoir jouer une nouvelle partie
     */
    void reset_all();


    /**
     * Permet d'utiliser une boucle for-each pour parcourir toutes les cellules du labyrinthe.
    */
    auto begin() const {return graph.begin();}

    /**
     * Permet d'utiliser une boucle for-each pour parcourir toutes les cellules du labyrinthe.
    */
    auto end() const {return graph.end();}

    /**
     * Accède à la cellule de coordonnée "c".
    */
	Cell &get_cell(const Coordinate &c);
	const Cell &get_cell(const Coordinate &c) const;

    /**
     * Renvoie la position actuelle du joueur.
    */
    Coordinate get_position() const;

	/**
 	 * Vérifie si toutes les cellules ont le même identifiant "id".
	 * Renvoie true si c'est le cas, false sinon.
	*/
	bool is_fusion_finished() const;

	/**
 	 * Met en place l'algorithme de construction de mur Fusion.
	 * Modifie le labyrinthe actuel.
	*/
	void fusion();

    /**
 	 * Met en place l'algorithme de construction de mur Aldous Broder.
	 * Modifie le labyrinthe actuel.
    */
   void construire_aldous_broder();


   /**
    * Resoud le labyrinthe et renvoie un vector indicant le chemin a suivre
    */
    std::vector<Coordinate> resolve_labyrinthe() const;

    friend std::ostream &operator<<(std::ostream &f, const Labyrinth &l)
    {
        for (const auto &cell: l)
        {
            f << cell.second << "\n";
        }
        return f;
    }
};
