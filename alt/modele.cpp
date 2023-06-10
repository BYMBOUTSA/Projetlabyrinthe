// fichier : modele.cpp
#include "modele.hpp"

// Méthodes de la classe Cell

// Renvoie les coordonnées de la cellule
const Coordinate &Cell::get_coordinate() const { return coordinate; }

// Renvoie l'identifiant de la cellule
int Cell::get_id() const { return id; }

// Définit un nouvel identifiant pour la cellule
void Cell::set_id(const int new_id) { id = new_id; }

// Ajoute une cellule voisine à la liste des voisins de la cellule actuelle
bool Cell::add_neighbour(const Cell &neigh) {
  return adgency_list.insert(neigh.coordinate).second;
}

// Supprime tous les voisins de la cellule
void Cell::remove_neighbour() { adgency_list.clear(); }

// Vérifie si une certaine coordonnée est un voisin de la cellule
bool Cell::is_neighbour(const Coordinate &coord) const {
  for (const auto &neigh_coord : adgency_list) {
    if (coord == neigh_coord) {
      return true;
    }
  }
  return false;
}

// Méthodes de la classe Labyrinth

// Déplace la position actuelle dans le labyrinthe en fonction de la direction
// donnée et renvoie une valeur indiquant si le déplacement a réussi ou s'il y a
// une victoire
int Labyrinth::move_position(const int side) {
  Coordinate current_coord = {currentPosition.getX(), currentPosition.getY()};

  Coordinate future_coord = {currentPosition.getX(), currentPosition.getY()};
  int return_value = 0;

  switch (side) {
  case 1:
    future_coord.second--;
    break;
  case 2:
    future_coord.first++;
    break;
  case 3:
    future_coord.second++;
    break;
  case 0:
    future_coord.first--;
    break;
  default:
    return 0;
  }
  if (is_in_graph(future_coord) && is_neighbour(current_coord, future_coord)) {
    // Le déplacement est confiné dans le labyrinthe ET les deux cellules n'ont
    // pas de mur les séparant
    return_value = 1;

    if (is_winning(future_coord)) {
      // Le joueur a gagné
      return_value = 2;
    }

    switch (side) {
    case 1:
      currentPosition.deplacerHaut();
      break;
    case 2:
      currentPosition.deplacerDroite();
      break;
    case 3:
      currentPosition.deplacerBas();
      break;
    case 0:
      currentPosition.deplacerGauche();
      break;
    default:
      break;
    }
  }
  return return_value;
}

// Vérifie si une certaine coordonnée est un voisin d'une autre coordonnée dans
// le labyrinthe
bool Labyrinth::is_neighbour(const Coordinate &coord1,
                             const Coordinate &coord2) const {
  return get_cell(coord1).is_neighbour(coord2);
}

// Vérifie si une certaine coordonnée correspond à la position de victoire
bool Labyrinth::is_winning(const Coordinate &coord) const {
  return coord == wining_coord;
}

// Vérifie si une certaine coordonnée est dans le labyrinthe
bool Labyrinth::is_in_graph(const Coordinate &coord) const {
  return coord.first >= 0 && coord.first < height && coord.second >= 0 &&
         coord.second < width;
}

// Supprime le mur entre deux cellules
void Labyrinth::remove_wall(Cell &cell1, Cell &cell2) {
  cell1.add_neighbour(cell2);
  cell2.add_neighbour(cell1);
}

// Supprime le mur entre deux coordonnées
void Labyrinth::remove_wall(const Coordinate &c1, const Coordinate &c2) {
  Cell &cell1 = get_cell(c1);
  Cell &cell2 = get_cell(c2);
  remove_wall(cell1, cell2);
}

// Réinitialise tous les murs du labyrinthe
void Labyrinth::reset_wall() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      get_cell({i, j}).remove_neighbour();
      get_cell({i, j}).set_id(j + i * width);
    }
  }
}

// Renvoie la cellule correspondant à une certaine coordonnée
Cell &Labyrinth::get_cell(const Coordinate &c) { return graph.at(c); }

// Renvoie la cellule correspondant à une certaine coordonnée (version
// constante)
const Cell &Labyrinth::get_cell(const Coordinate &c) const {
  return graph.at(c);
}

// Renvoie la position actuelle du joueur dans le labyrinthe
Coordinate Labyrinth::get_position() const {
  return {currentPosition.getX(), currentPosition.getY()};
}

// Vérifie si la fusion des cellules du labyrinthe est terminée
bool Labyrinth::is_fusion_finished() const {
  const int valid_id = get_cell({0, 0}).get_id();

  for (const auto &pair : graph) {
    if (pair.second.get_id() != valid_id) {
      return false;
    }
  }
  return true;
}

// Effectue la fusion des cellules du labyrinthe
void Labyrinth::fusion() {
  while (!is_fusion_finished()) {
    // Sélectionne une coordonnée aléatoirement incluse dans le labyrinthe
    const Coordinate coord1 = {std::rand() % height, std::rand() % width};
    Coordinate coord2;
    do {
      // Sélectionne aléatoirement une coordonnée voisine de la première
      // en prenant une direction cardinale aléatoire
      // (0 à 3 dans le sens horaire en commençant au Nord)
      coord2.first = coord1.first;
      coord2.second = coord1.second;
      const int side = std::rand() & 3;

      switch (side) {
      case 0:
        coord2.first = coord1.first - 1;
        break;
      case 1:
        coord2.second = coord1.second - 1;
        break;
      case 2:
        coord2.first = coord1.first + 1;
        break;
      case 3:
        coord2.second = coord1.second + 1;
        break;
      }

    } while (!is_in_graph(coord2));

    // Les deux coordonnées sont voisines et dans le labyrinthe
    const int id_to_spread = get_cell(coord1).get_id();

    if (id_to_spread != get_cell(coord2).get_id()) {
      // Les ids sont différents
      // On ouvre le mur entre les deux
      remove_wall(coord1, coord2);

      // Il faut égaliser les ids des deux chemins partant de ces deux
      // coordonnées
      std::list<Coordinate> coord_to_propagate;
      std::set<Coordinate> coord_done;

      coord_to_propagate.push_back(coord2);

      while (!coord_to_propagate.empty()) {
        // On récupère la première cellule de la liste à faire
        Coordinate current_coord = coord_to_propagate.front();
        Cell &current_cell = get_cell(current_coord);

        // On la supprime de la liste à faire
        coord_to_propagate.pop_front();
        // On l'ajoute à la liste des cellules faites
        coord_done.insert(current_coord);
        get_cell(current_coord).set_id(id_to_spread);

        for (Coordinate coord_neighbour : current_cell) {
          // On parcourt tous les voisins de la cellule courante
          // On met à jour leur id

          if (coord_done.find(coord_neighbour) == coord_done.end()) {
            // coord_neighbour has not been computed
            coord_to_propagate.push_back(coord_neighbour);
          }
        }
      }
    }
  }
}

// Construction du labyrinthe en utilisant l'algorithme d'Aldous-Broder
void Labyrinth::construire_aldous_broder() {
  // Sélectionne une cellule de départ aléatoirement
  uint32_t random_start_y = 1 + rand() % (static_cast<int>(height) - 2);
  uint32_t random_start_x = 1 + rand() % (static_cast<int>(width) - 2);

  uint32_t current_y = (random_start_y / 2) * 2 + 1;
  uint32_t current_x = (random_start_x / 2) * 2 + 1;

  // Attribue un identifiant à la cellule de départ
  get_cell({current_y, current_x}).set_id(0);
  uint32_t total_cells = (height / 2) * (width / 2) - 1;

  while (total_cells > 0) {
    // Sélectionne une direction aléatoire (0 à 3)
    uint32_t next_direction = rand() % 4;

    // Calcule les coordonnées de la prochaine cellule dans la direction choisie
    uint32_t next_y = current_y;
    uint32_t next_x = current_x;

    if (next_direction == 0) {
      next_y -= 2;
      if (next_y >= 1 && get_cell({next_y, current_x}).get_id() == -1) {
        // La prochaine cellule est valide et non visitée
        total_cells--;
        remove_wall({next_y, current_x}, {next_y + 1, current_x});
        get_cell({next_y, current_x})
            .set_id(get_cell({current_y, current_x}).get_id());
      }
    } else if (next_direction == 1) {
      next_y += 2;
      if (next_y <= static_cast<uint32_t>(height) - 2 &&
          get_cell({next_y, current_x}).get_id() == -1) {
        total_cells--;
        remove_wall({next_y, current_x}, {next_y - 1, current_x});
        get_cell({next_y, current_x})
            .set_id(get_cell({current_y, current_x}).get_id());
      }
    } else if (next_direction == 2) {
      next_x -= 2;
      if (next_x >= 1 && get_cell({current_y, next_x}).get_id() == -1) {
        total_cells--;
        remove_wall({current_y, next_x}, {current_y, next_x + 1});
        get_cell({current_y, next_x})
            .set_id(get_cell({current_y, current_x}).get_id());
      }
    } else if (next_direction == 3) {
      next_x += 2;
      if (next_x <= static_cast<uint32_t>(width) - 2 &&
          get_cell({current_y, next_x}).get_id() == -1) {
        total_cells--;
        remove_wall({current_y, next_x}, {current_y, next_x - 1});
        get_cell({current_y, next_x})
            .set_id(get_cell({current_y, current_x}).get_id());
      }
    }

    current_y = next_y;
    current_x = next_x;
  }

  // Sélectionne une cellule de fin aléatoirement sur le bord du labyrinthe
  uint32_t random_end_y = rand() % height;
  uint32_t random_end_x = rand() % width;

  if (random_end_y % 2 == 0) {
    random_end_x = (random_end_x / 2) * 2 + 1;
  } else {
    random_end_x = random_end_x / 2 * 2;
  }

  if (random_end_x % 2 == 0) {
    random_end_y = (random_end_y / 2) * 2 + 1;
  } else {
    random_end_y = random_end_y / 2 * 2;
  }

  // Attribue un identifiant différent à la cellule de fin
  get_cell({random_end_y, random_end_x}).set_id(1);
  wining_coord = {random_end_y, random_end_x};

  // Initialise la position actuelle du joueur
  currentPosition = Position(random_start_x, random_start_y);
}
