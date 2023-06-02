#include "modele.hpp"

const Coordinate &Cell::get_coordinate() const { return coordinate; }

int Cell::get_id() const { return id; }

void Cell::set_id(const int new_id) { id = new_id; }

bool Cell::add_neighbour(const Cell &neigh) {
  return adgency_list.insert(neigh.coordinate).second;
}

void Cell::remove_neighbour() { adgency_list.clear(); }

void Labyrinth::remove_wall(Cell &cell1, Cell &cell2) {
  cell1.add_neighbour(cell2);
  cell2.add_neighbour(cell1);
}

bool Labyrinth::is_in_graph(const Coordinate &coord) const {
  return coord.first >= 0 && coord.first < height && coord.second >= 0 &&
         coord.second < width;
}

void Labyrinth::remove_wall(const Coordinate &c1, const Coordinate &c2) {
  Cell &cell1 = get_cell(c1);
  Cell &cell2 = get_cell(c2);
  remove_wall(cell1, cell2);
}

void Labyrinth::reset_wall() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      get_cell({i, j}).remove_neighbour();
    }
  }
}

Cell &Labyrinth::get_cell(const Coordinate &c) { return graph.at(c); }

const Cell &Labyrinth::get_cell(const Coordinate &c) const {
  return graph.at(c);
}

bool Labyrinth::is_fusion_finished() const {
  const int valid_id = get_cell({0, 0}).get_id();

  for (const auto &pair : graph) {
    if (pair.second.get_id() != valid_id) {
      return false;
    }
  }
  return true;
}

void Labyrinth::fusion() {
  while (!is_fusion_finished()) {

    // Selectionne une coordonnée aléatoirement inclus dans le labyrinthe
    const Coordinate coord1 = {std::rand() % height, std::rand() % width};
    Coordinate coord2;
    do {
      // Selectionne aléatoirement une coordonnée voisine de la première
      // en prenant une direction cardinale aléatoire
      // (0 à 3 dans le sens horaire en commancant au Nord)
      coord2.first = coord1.first;
      coord2.second = coord1.second;
      const int side = std::rand() & 3;
      // DEBUG std::cout << side << '\n';
      switch (side) {
      case 0:
        coord2.first = coord1.first - 1;
        break;
      case 1:
        coord2.second = coord1.second + 1;
        break;
      case 2:
        coord2.first = coord1.first + 1;
        break;
      case 3:
        coord2.second = coord1.second - 1;
        break;
      }
      // DEBUG std::cout << "test: (" << coord2.first << "," << coord2.second <<
      // ")\n";
    } while (!is_in_graph(coord2));

    // Les deux coordonnées sont voisines et dans le labyrinthe
    // DEBUG std::cout << "Cellule: (" << coord1.first << "," << coord1.second
    // << ") voisin de " <<
    // DEBUG	"(" << coord2.first << "," << coord2.second << ")\n";

    const int id_to_spread = get_cell(coord1).get_id();
    // DEBUG std::cout << "id to spread: " << id_to_spread << '\n';

    if (id_to_spread != get_cell(coord2).get_id()) {
      // Les ids sont différents
      // On ouvre le mur entre les 2
      remove_wall(coord1, coord2);

      // il faut égaliser les ids des 2 chemins partant de ces 2 coordonnées
      std::list<Coordinate> coord_to_propagate;
      std::set<Coordinate> coord_done;

      coord_to_propagate.push_back(coord2);

      while (!coord_to_propagate.empty()) {
        // On recupere la première cellule de la liste à faire
        Coordinate current_coord = coord_to_propagate.front();
        Cell &current_cell = get_cell(current_coord);

        // DEBUG std::cout << "début de la boucle while avec la cellule:" <<
        // current_cell << '\n';

        // On la supprime de la liste a faire
        coord_to_propagate.pop_front();
        // On l'ajoute à la liste des cellules faites
        coord_done.insert(current_coord);
        get_cell(current_coord).set_id(id_to_spread);

        for (Coordinate coord_neighbour : current_cell) {
          // On parcours tous les voisins de la cellule courante
          // On met a jour leur id

          // DEBUG std::cout << "celulle voisine: " << (*this)(coord_neighbour)
          // << '\n';

          if (coord_done.find(coord_neighbour) == coord_done.end()) {
            // coord_neighbour has not been computed
            coord_to_propagate.push_back(coord_neighbour);

            // DEBUG std::cout << "Valeur modifié: " << (*this)(coord_neighbour)
            // << '\n';
          }
        }
      }
    }
  }
}


void Labyrinth::construire_aldous_broder() {
    while (!is_fusion_finished()) {
        const Coordinate coord1 = {std::rand() % height, std::rand() % width};
        Coordinate coord2;
        do {
            coord2.first = coord1.first;
            coord2.second = coord1.second;
            const int side = std::rand() % 4;
            switch (side) {
                case 0: coord2.first = coord1.first - 1; break;
                case 1: coord2.second = coord1.second + 1; break;
                case 2: coord2.first = coord1.first + 1; break;
                case 3: coord2.second = coord1.second - 1; break;
            }
        } while (!is_in_graph(coord2));

        const int id_to_spread = get_cell(coord1).get_id();

        if (id_to_spread != get_cell(coord2).get_id()) {
            remove_wall(coord1, coord2);

            std::list<Coordinate> coord_to_propagate;
            std::set<Coordinate> coord_done;

            coord_to_propagate.push_back(coord2);

            while (!coord_to_propagate.empty()) {
                Coordinate current_coord = coord_to_propagate.front();
                Cell& current_cell = get_cell(current_coord);

                coord_to_propagate.pop_front();
                coord_done.insert(current_coord);
                get_cell(current_coord).set_id(id_to_spread);

                for (Coordinate coord_neighbour : current_cell) {
                    if (coord_done.find(coord_neighbour) == coord_done.end()) {
                        coord_to_propagate.push_back(coord_neighbour);
                    }
                }
            }
        }
    }
}