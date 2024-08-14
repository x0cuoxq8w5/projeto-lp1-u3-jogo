// 2

#ifndef PROJETIL_HPP
#define PROJETIL_HPP

#include "Bola.hpp"
#include <vector>

using namespace sf;

class Projetil : public Bola {
    float movespeed = 10; // Velocidade do projetil
public:
    int time;
    bool clrflag = false;

    // Construtor do projetil
    Projetil(Vector2f target, Vector2f pos, int team);
    Projetil(Vector2f target, Vector2f pos, int team, float tiro);
    void update();
};

#endif