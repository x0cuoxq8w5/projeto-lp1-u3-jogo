// 8

#ifndef GAMETEXT_HPP
#define GAMETEXT_HPP

#include "Player.hpp"
#include "Base.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class GameText{
    Font fonte;
    Player *player;
    Base *base;
    Time *tempojogoatual;

public:
    Text hpplayer{"DEFAULT", fonte, 30}, hpbase{"DEFAULT", fonte, 30},
        relogio{"DEFAULT", fonte, 30}, muni{"DEFAULT", fonte, 30},
        dinheiro{"DEFAULT", fonte, 30}, upgrades{"DEFAULT", fonte, 30},
        ganhou{"Ganhou!", fonte, 64}, perdeu{"Perdeu!", fonte, 64};

    GameText(Player *player1, Base *base1, Time *tempojogo);
    void update();
    void calculaRelogio();
};

#endif