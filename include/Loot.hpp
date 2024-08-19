// 3

#ifndef LOOT_HPP
#define LOOT_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

class Loot {
public:
    RectangleShape sprite{{40,30}}; // Sprite do loot com tamanho 40x30
    int dinheiro, muni; // Quantidade de dinheiro e munição no loot
    Clock despawntimer; // Timer para o despawn do loot
    Time elapsed = seconds(0);
    bool clrflag = false;

    // Construtor do loot
    Loot(int cash, int ammo, Vector2f pos);
    void update();
    void pausado();
    void despausado();
};

#endif