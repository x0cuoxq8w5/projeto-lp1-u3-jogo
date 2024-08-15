// 7

#ifndef INIMIGO_HPP
#define INIMIGO_HPP

#include "Shooter.hpp"
#include "Player.hpp"
#include <cmath>
#include <random>

using namespace std;
using namespace sf;

class Inimigo : public Shooter {
private:
    float movespeed = 5;
    Clock shoottimer;
    Time elapsed = milliseconds(0);
    Player *playeraddr;
    int dinheiro, muni;
public:
    bool clrflag = false;;
    Inimigo(float angulo, float delay, std::vector<Projetil> *proj, Player *jogador, Base *bas, int money, int bullet, std::vector<Loot>* pickup);
    void update();
    void atirar();
    void spawnarloot();
    void pausado();
    void despausado();
};

#endif
