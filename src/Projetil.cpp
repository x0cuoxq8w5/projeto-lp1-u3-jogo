#include "Projetil.hpp"
#include "Globals.hpp"
#include <cmath>

Projetil::Projetil(Vector2f target, Vector2f pos, int team) {
    if (target - pos == Vector2f{0, 0}) {
        target.x++;
        target.y++;
    }
    versoralvo = calculaVersor(target - pos);
    velocidade = versoralvo * movespeed;
    sprite.setPosition(pos);
    sprite.setRadius(raioprojetil);
    sprite.setFillColor(Color::Yellow);
    sprite.setOrigin(raioprojetil, raioprojetil);
    time = team;
}

Projetil::Projetil(Vector2f target, Vector2f pos, int team, float tiro) : Projetil(target, pos, team) {
    movespeed += tiro;
}

void Projetil::update() {
    elapsed += despawntimer.getElapsedTime();
    despawntimer.restart();
    sprite.move(velocidade);
    if (sprite.getPosition().x < 0 || sprite.getPosition().x > dimensoestela.x || sprite.getPosition().y < 0
    || sprite.getPosition().y > dimensoestela.y || elapsed.asSeconds() > 2) {
        clrflag = true;
    }
}
void Projetil::pausado() {
    elapsed += despawntimer.getElapsedTime();
}

void Projetil::despausado() {
    despawntimer.restart();
}