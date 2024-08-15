// 4

#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Projetil.hpp"
#include "Globals.hpp"

using namespace sf;

class Base {
    Clock regentimer;
    Time elapsed;
    int hpmax = 1;
    bool* loseflagaddr;
    vector<Projetil>* projaddr;
public:
    int hp = 100, regen = 0;
    RectangleShape sprite{{275,125}};
    Base(bool* loseflag, vector<Projetil>* endproj);
    void update();
    void levardano();
    void pausado();
    void despausado();
};

#endif