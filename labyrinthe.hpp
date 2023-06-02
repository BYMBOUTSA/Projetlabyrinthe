#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

class Labyrinthe : public Graphe {
private:
    static const std::vector<std::string> coins;
    int largeur;
    int hauteur;
    std::vector<std::vector<char>> representation;
    std::vector<std::pair<int, int>> ouvertures;

public:
    Labyrinthe(int largeur = 0, int hauteur = 0) : Graphe(false), largeur(largeur), hauteur(hauteur) {
        reset();
        representation = std::vector<std::vector<char>>(2 * hauteur + 1, std::vector<char>(2 * largeur + 1, '*'));
        effacerRepresentation();
    }

    void reset() {
        A.clear();
        for (int ligne = 0; ligne < hauteur; ligne++) {
            for (int colonne = 0; colonne < largeur; colonne++) {
                ajouterSommet(std::make_pair(ligne, colonne));
            }
        }
    }

    std::string toString() {
        construireRepresentation();
        std::string str;
        for (const auto& ligne : representation) {
            str += std::string(ligne.begin(), ligne.end()) + "\n";
        }
        return str;
    }

    void construireRepresentation() {
        for (int colonne = 0; colonne < largeur; colonne++) {
            representation[0][2 * colonne + 1] = coins[5][0];
        }
        for (int ligne = 0; ligne < hauteur; ligne++) {
            representation[2 * ligne + 1][0] = coins[10][0];
            for (int colonne = 0; colonne < largeur; colonne++) {
                if (ligne + 1 < hauteur && arete(std::make_pair(ligne, colonne), std::make_pair(ligne + 1, colonne))) {
                    representation[2 * ligne + 2][2 * colonne + 1] = coins[0][0];
                } else {
                    representation[2 * ligne + 2][2 * colonne + 1] = coins[5][0];
                }

                if (colonne + 1 < largeur && arete(std::make_pair(ligne, colonne), std::make_pair(ligne, colonne + 1))) {
                    representation[2 * ligne + 1][2 * colonne + 2] = coins[0][0];
                } else {
                    representation[2 * ligne + 1][2 * colonne + 2] = coins[10][0];
                }
            }
        }

        for (int ligne = 0; ligne < representation.size(); ligne += 2) {
            for (int colonne = 0; colonne < representation[0].size(); colonne += 2) {
                int code = 1 * (colonne + 1 < representation[0].size() && representation[ligne][colonne + 1] != ' ');
                code += 2 * (ligne != 0 && representation[ligne - 1][colonne] != ' ');
                code += 4 * (colonne != 0 && representation[ligne][colonne - 1] != ' ');
                code += 8 * (ligne + 1 < representation.size() && representation[ligne + 1][colonne] != ' ');
                representation[ligne][colonne] = coins[code][0];
            }
        }

        for (const auto& ouverture : ouvertures) {
            int ligne = ouverture.first;
            int colonne = ouverture.second;
            if (colonne == 0) {
                representation[2 * ligne + 1][2 * colonne] = ' ';
            } else if (colonne == largeur - 1) {
                representation[2 * ligne + 1][2 * colonne + 2] = ' ';
            } else if (ligne == 0) {
                representation[2 * ligne][2 * colonne + 1] = ' ';
            } else if (ligne == hauteur - 1) {
                representation[2 * ligne + 2][2 * colonne + 1] = ' ';
            }
        }
    }

    void effacerRepresentation() {
        for (int ligne = 0; ligne < hauteur; ligne++) {
            for (int colonne = 0; colonne < largeur; colonne++) {
                representation[2 * ligne + 1][2 * colonne + 1] = ' ';
            }
        }
    }

    static const std::vector<std::string> coins;
};

const std::vector<std::string> Labyrinthe::coins = {
    " ", "═", "║", "╚", "═", "═", "╝", "╩", "║", "╔", "║", "╠", "╗", "╦", "╣", "╬"
};

int main() {
    Labyrinthe labyrinthe(5, 5);
    labyrinthe.ajouterArete(std::make_pair(0, 0), std::make_pair(0, 1));
    labyrinthe.ajouterArete(std::make_pair(0, 0), std::make_pair(1, 0));
    labyrinthe.ajouterArete(std::make_pair(1, 0), std::make_pair(1, 1));
    labyrinthe.ajouterArete(std::make_pair(1, 1), std::make_pair(2, 1));
    labyrinthe.ajouterArete(std::make_pair(2, 1), std::make_pair(2, 2));
    labyrinthe.ajouterArete(std::make_pair(2, 2), std::make_pair(3, 2));
    labyrinthe.ajouterArete(std::make_pair(3, 2), std::make_pair(3, 3));
    labyrinthe.ajouterArete(std::make_pair(3, 3), std::make_pair(4, 3));
    labyrinthe.ajouterArete(std::make_pair(4, 3), std::make_pair(4, 4));
    labyrinthe.ajouterArete(std::make_pair(0, 1), std::make_pair(0, 2));
    labyrinthe.ajouterArete(std::make_pair(0, 2), std::make_pair(1, 2));
    labyrinthe.ajouterArete(std::make_pair(1, 2), std::make_pair(1, 3));
    labyrinthe.ajouterArete(std::make_pair(1, 3), std::make_pair(2, 3));
    labyrinthe.ajouterArete(std::make_pair(2, 3), std::make_pair(2, 4));
    labyrinthe.ajouterArete(std::make_pair(0, 1), std::make_pair(1, 1));
    labyrinthe.ajouterArete(std::make_pair(0, 2), std::make_pair(0, 3));
    labyrinthe.ajouterArete(std::make_pair(0, 3), std::make_pair(1, 3));
    labyrinthe.ajouterArete(std::make_pair(0, 3), std::make_pair(0, 4));
    std::cout << labyrinthe.toString() << std::endl;
    return 0;
}
