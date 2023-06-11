// fichier : observateur.hpp
#pragma once

template <typename T>
class Observateur
{
public:
    /**
     * Méthode virtuelle pure qui sera implémentée par les classes dérivées.
     * Cette méthode est appelée lorsqu'une mise à jour est effectuée et reçoit l'information mise à jour en paramètre.
     */
    virtual void update(const T &info) = 0;
};
