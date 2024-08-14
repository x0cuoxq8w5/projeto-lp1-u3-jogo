#include "Loot.hpp"

Loot::Loot(int cash, int ammo, Vector2f pos) {
    dinheiro = cash;
    muni = ammo;
    sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
    sprite.setPosition(pos);
    sprite.setFillColor(Color::Yellow);
}

void Loot::update() {
    elapsed += despawntimer.getElapsedTime();
    despawntimer.restart();
    if (elapsed.asSeconds() > 5) {
        clrflag = true;
    }
}

void Loot::pausado() {
    elapsed += despawntimer.getElapsedTime();
}

void Loot::despausado() {
    despawntimer.restart();
}