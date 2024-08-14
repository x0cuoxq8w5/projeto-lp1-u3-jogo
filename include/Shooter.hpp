// 5

#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "Bola.hpp"
#include "Projetil.hpp"
#include "Loot.hpp"
#include "Base.hpp"
#include <vector>

using namespace std;

class Shooter : public Bola {
protected:
    vector<Projetil>* projeteisaddr;
    vector<Loot>* lootaddr;
    Base* baseaddr;
};

#endif