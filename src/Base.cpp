#include "Base.hpp"
#include "Globals.hpp"

Base::Base(bool* loseflag) {
    loseflagaddr = loseflag;
    sprite.setFillColor(Color(127, 127, 192));
    sprite.setPosition(centrotela);
    sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
}

void Base::update() {
    if (hp < hpmax) {
        if (elapsed.asMilliseconds() > 1000) {
            elapsed = milliseconds(0);
            hp += regen;
        }
    }
    if (hp == 0) {
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