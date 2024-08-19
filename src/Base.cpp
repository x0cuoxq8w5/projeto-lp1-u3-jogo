#include "Base.hpp"
#include "Globals.hpp"
#include "Projetil.hpp"

Base::Base(bool* loseflag, vector<Projetil>* endproj) {
    loseflagaddr = loseflag;
    projaddr = endproj;
    sprite.setFillColor(Color(127, 127, 192));
    sprite.setPosition(centrotela);
    sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
}

void Base::update() {
    for (Projetil & proj : *projaddr) {
        if (proj.time == 1) {
            if (sprite.getGlobalBounds().intersects(proj.sprite.getGlobalBounds())) {
                proj.clrflag = true;
                levardano();
            }
        }
    }
    if (hp < hpmax) {
        if (elapsed.asMilliseconds() > 1000) {
            elapsed = milliseconds(0);
            hp += min(regen, hpmax-hp);
        }
    }
    if (hp <= 0) {
        *loseflagaddr = true;
    }
    elapsed += regentimer.getElapsedTime();
    regentimer.restart();
}

void Base::levardano() {
    hp--;
}

void Base::pausado() {
    elapsed += regentimer.getElapsedTime();
}

void Base::despausado() {
    regentimer.restart();
}