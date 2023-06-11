// Fichier : position.hpp
#pragma once

class Position {
private:
    int x; // Coordonnée x de la position
    int y; // Coordonnée y de la position

public:
    /**
     * Constructeur de la classe Position.
     * Initialise les coordonnées x et y de la position avec les valeurs fournies.
     *
     * @param startX La coordonnée x initiale de la position.
     * @param startY La coordonnée y initiale de la position.
     */
    Position(int startX, int startY) : x(startX), y(startY) {}

    /**
     * Méthode getter pour obtenir la coordonnée x de la position.
     *
     * @return La coordonnée x de la position.
     */
    int getX() const { return x; }

    /**
     * Méthode getter pour obtenir la coordonnée y de la position.
     *
     * @return La coordonnée y de la position.
     */
    int getY() const { return y; }

    void setX(const int newX) {x = newX;}

    void setY(const int newY) {y = newY;}

    /**
     * Déplace la position vers la gauche en décrémentant la coordonnée x.
     */
    void deplacerGauche() { x--; }

    /**
     * Déplace la position vers la droite en incrémentant la coordonnée x.
     */
    void deplacerDroite() { x++; }

    /**
     * Déplace la position vers le haut en décrémentant la coordonnée y.
     */
    void deplacerHaut() { y--; }

    /**
     * Déplace la position vers le bas en incrémentant la coordonnée y.
     */
    void deplacerBas() { y++; }
};
