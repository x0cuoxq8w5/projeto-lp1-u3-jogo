// 4

#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

class Base {
    Clock regentimer;
    Time elapsed;
    int hpmax = 100;
    bool* loseflagaddr;
public:
    int hp = 100, regen = 0;
    RectangleShape sprite{{275,125}};
    explicit Base(bool* loseflag);
    void update();
    void levardano();
    void pausado();
    void despausado();
};

#endif