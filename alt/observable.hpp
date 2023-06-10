// fichier : observable.hpp
#pragma once

#include <list>
#include "observateur.hpp"

template <typename T>
class Observable
{
private:
    std::list<Observateur<T> *> list_observateurs;

public:
    /**
     * Notifie tous les observateurs en appelant leur méthode "update" avec l'information fournie.
     */
    void notifierObservateurs(T info)
    {
        for (const auto &obs : this->list_observateurs)
            obs->update(info);
    }

    /**
     * Ajoute un observateur à la liste des observateurs.
     */
    void ajouterObservateur(Observateur<T> *observateur)
    {
        this->list_observateurs.push_back(observateur);
    }
};
