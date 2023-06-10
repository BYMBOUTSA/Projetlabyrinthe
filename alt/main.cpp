#include "controller.hpp"
#include "modele.hpp"
#include "vue.hpp"
#include <gtkmm/application.h>
#include <iostream>
#include <map>

int main(int argc, char **argv) {
  std::srand(
      std::time(nullptr)); // Initialise le générateur de nombres aléatoires
                           // avec une graine basée sur l'horloge du système

  auto app = Gtk::Application::create(
      argc, argv); // Crée une instance de l'application Gtk

  Controller c; // Crée une instance du contrôleur

  return app->run(c.get_vue()); // Lance l'exécution de l'application Gtk en
                                // exécutant la méthode run() de l'application
                                // sur l'objet Vue obtenu à partir du contrôleur
}
