#include "modele.hpp"
#include <iostream>


int main()
{
    Cell c1({0, 0});
    Cell c2({1, 0});
    Cell c3({0, 1});

    std::cout << c1 << "\n";
    c1.add_neighbour(c2);
    c1.add_neighbour(c3);

    std::cout << c1 << "\n";

    Labyrinth l(3, 3);
    std::cout << "==========\n" << l << "\n";
    l.remove_wall({0, 0}, {0, 1});

    std::cout << "\n" << l << "\n";
    l.reset_wall();
    std::cout << "\n" << l << "\n";

	std::cout << "Lancement de l'algorithme fusion\n";
	l.fusion();
	std::cout << "Fusion terminé: \n" << l << '\n';
}