// 6

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Shooter.hpp"
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Player : public Shooter {
    bool* loseflagaddr;
public:
    int hp = 100, dinheiro = 0, muni = 25, precos[3] = {10, 20, 30};
    float shotspeed = 0;
    Window *janela;

    // Construtor do jogador
    Player(float posX, float posY, Window *vindow, vector<Projetil> *proj, vector<Loot>* drops, Base* base1, bool *loseflag);
    void update();
    void atirar();
    void calculoMovimento();
    void calculoColisao();
    void upgrade(int qual);
};

#endif