//#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

class Graphe {
    private:
        std::unordered_map<int, std::unordered_set<int>> adjacence;
        bool oriente;

    public:
        Graphe(bool estOriente = true) : oriente(estOriente) {}

        void construire(std::unordered_map<int, std::unordered_set<int>> adjacence) {
            this->adjacence = adjacence;
        }

        void ajouterSommet(int sommet) {
            if (adjacence.find(sommet) == adjacence.end()) {
                adjacence[sommet] = std::unordered_set<int>();
            }
        }

        void ajouterArete(int sommet1, int sommet2) {
            ajouterSommet(sommet1);
            ajouterSommet(sommet2);
            adjacence[sommet1].insert(sommet2);
            if (!oriente) {
                adjacence[sommet2].insert(sommet1);
            }
        }

        std::unordered_set<int> voisins(int sommet) {
            return adjacence[sommet];
        }

        std::vector<std::pair<int, int>> aretes() {
            std::vector<std::pair<int, int>> listeAretes;
            for (const auto& entree : adjacence) {
                int sommet = entree.first;
                const auto& voisins = entree.second;
                for (int voisin : voisins) {
                    listeAretes.push_back(std::make_pair(sommet, voisin));
                }
            }
            return listeAretes;
        }

        bool estArete(int sommet1, int sommet2) {
            return adjacence[sommet1].count(sommet2) > 0;
        }

        std::unordered_set<int> parcoursProfondeur(int sommet, std::unordered_set<int>& vus) {
            if (vus.count(sommet) == 0) {
                vus.insert(sommet);
                for (int voisin : voisins(sommet)) {
                    parcoursProfondeur(voisin, vus);
                }
            }
            return vus;
        }

        std::unordered_map<int, int> parcoursChemin(int sommet, std::unordered_map<int, int>& vus) {
            if (vus.count(sommet) == 0) {
                vus[sommet] = -1;
                for (int voisin : voisins(sommet)) {
                    parcoursChemin(voisin, vus);
                }
            }
            return vus;
        }

        std::vector<int> chemins(int sommet1, int sommet2) {
            std::unordered_map<int, int> vus;
            vus = parcoursChemin(sommet1, vus);
            std::vector<int> chemin;
            if (vus.count(sommet2) > 0) {
                int sommet = sommet2;
                while (sommet != -1) {
                    chemin.push_back(sommet);
                    sommet = vus[sommet];
                }
            }
            std::reverse(chemin.begin(), chemin.end());
            return chemin;
        }

        bool existeChemin(int sommet1, int sommet2) {
            std::unordered_set<int> vus;
            vus = parcoursProfondeur(sommet1, vus);
            return vus.count(sommet2) > 0;
        }

        std::unordered_map<int, int> parcoursLargeur(int sommet) {
            std::unordered_map<int, int> distances;
            std::unordered_set<int> courant;
            std::unordered_set<int> suivant;
            courant.insert(sommet);
            int distanceCourante = 0;
            while (!courant.empty()) {
                int noeud = *(courant.begin());
                courant.erase(courant.begin());
                distances[noeud] = distanceCourante;
                distanceCourante++;
                for (int voisin : voisins(noeud)) {
                    if (distances.count(voisin) == 0) {
                        suivant.insert(voisin);
                    }
                }
                if (courant.empty()) {
                    courant = suivant;
                    suivant.clear();
                }
            }
            return distances;
        }

        std::unordered_map<int, int> parcoursLargeurChemin(int sommet) {
            std::unordered_map<int, int> vus;
            std::unordered_set<int> courant;
            std::unordered_set<int> suivant;
            courant.insert(sommet);
            while (!courant.empty()) {
                int noeud = *(courant.begin());
                courant.erase(courant.begin());
                for (int voisin : voisins(noeud)) {
                    if (vus.count(voisin) == 0) {
                        suivant.insert(voisin);
                        vus[voisin] = noeud;
                    }
                }
                if (courant.empty()) {
                    courant = suivant;
                    suivant.clear();
                }
            }
            return vus;
        }

        std::vector<int> cheminsLargeur(int sommet1, int sommet2) {
            std::unordered_map<int, int> vus;
            vus = parcoursLargeurChemin(sommet1);
            std::vector<int> chemin;
            if (vus.count(sommet2) > 0) {
                int sommet = sommet2;
                while (sommet != -1) {
                    chemin.push_back(sommet);
                    sommet = vus[sommet];
                }
            }
            std::reverse(chemin.begin(), chemin.end());
            return chemin;
        }

        int distance(int sommet1, int sommet2) {
            std::unordered_map<int, int> distances;
            distances = parcoursLargeur(sommet1);
            if (distances.count(sommet2) > 0) {
                return distances[sommet2];
            }
            return -1;
        }
};

int main() {
    Graphe graphe;
    graphe.ajouterArete(1, 2);
    graphe.ajouterArete(1, 3);
    graphe.ajouterArete(2, 4);
    graphe.ajouterArete(3, 4);
    graphe.ajouterArete(4, 5);
    std::vector<std::pair<int, int>> aretes = graphe.aretes();
    for (const auto& arete : aretes) {
        std::cout << "(" << arete.first << ", " << arete.second << ")" << std::endl;
    }
    std::unordered_set<int> voisins = graphe.voisins(4);
    for (int voisin : voisins) {
        std::cout << voisin << " ";
    }
    std::cout << std::endl;
    std::cout << graphe.estArete(2, 4) << std::endl;
    std::unordered_set<int> parcoursProfondeur = graphe.parcoursProfondeur(1);
    for (int sommet : parcoursProfondeur) {
        std::cout << sommet << " ";
    }
    std::cout << std::endl;
    std::unordered_map<int, int> parcoursChemin = graphe.parcoursChemin(1);
    for (const auto& entree : parcoursChemin) {
        std::cout << entree.first << ": " << entree.second << std::endl;
    }
    std::vector<int> chemins = graphe.chemins(1, 5);
    for (int sommet : chemins) {
        std::cout << sommet << " ";
    }
    std::cout << std::endl;
    std::cout << graphe.existeChemin(1, 5) << std::endl;
    std::unordered_map<int, int> parcoursLargeur = graphe.parcoursLargeur(1);
    for (const auto& entree : parcoursLargeur) {
        std::cout << entree.first << ": " << entree.second << std::endl;
    }
    std::unordered_map<int, int> parcoursLargeurChemin = graphe.parcoursLargeurChemin(1);
    for (const auto& entree : parcoursLargeurChemin) {
        std::cout << entree.first << ": " << entree.second << std::endl;
    }
    std::vector<int> cheminsLargeur = graphe.cheminsLargeur(1, 5);
    for (int sommet : cheminsLargeur) {
        std::cout << sommet << " ";
    }
    std::cout << std::endl;
    std::cout << graphe.distance(1, 5) << std::endl;
    return 0;
}
