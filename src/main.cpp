// g++ -I./include ./src/Globals.cpp ./src/Bola.cpp ./src/Projetil.cpp ./src/Loot.cpp ./src/Base.cpp ./src/Shooter.cpp ./src/Player.cpp ./src/Inimigo.cpp ./src/GameText.cpp ./src/Funcoes.cpp ./src/main.cpp -o jogo -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Inimigo.hpp"
#include "GameText.hpp"
#include "Funcoes.hpp"

using namespace std;
using namespace sf;

#include <iostream>

#include "Base.hpp"
#include "Loot.hpp"
#include "Projetil.hpp"
#include <random>
#include <vector>
#include "Globals.hpp"


int main() {
    auto window = sf::RenderWindow{ { 1600u, 900u }, "Amo videogames"};
    
    window.setFramerateLimit(60);
    vector<Projetil> projeteis;
    vector<Inimigo> inimigos;
    vector<Loot> drops;
    bool pauseflag = false, loseflag = false;
    Base base(&loseflag, &projeteis);
    Player player(centrotela.x, centrotela.y, &window, &projeteis, &drops, &base, &loseflag);
    Clock timerinimigo, timerjogo;
    Time timeelapsed = seconds(0), tempoinimigo = milliseconds(0);
    random_device isca;
    mt19937 randomizer(isca());
    GameText textos(&player, &base, &timeelapsed);

    
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Escape: {
                        if (timeelapsed > maxgametime || loseflag) {
                            break;
                        }
                        if (pauseflag) {
                            pauseflag = false;
                            timerjogo.restart();
                            timerinimigo.restart();
                            despausador(inimigos,drops,base, projeteis);
                        } else {
                            pauseflag = true;
                            timeelapsed += timerjogo.getElapsedTime();
                            timerjogo.restart();
                            timerinimigo.restart();
                            pausador(inimigos, drops, base, projeteis);
                        }
                        break;
                    }
                    case Keyboard::Q: {
                        if (!pauseflag) {
                            player.atirar();
                        }
                        break;
                    }
                    case Keyboard::Num1: {
                        player.upgrade(0);
                        break;
                    }
                    case Keyboard::Num2: {
                        player.upgrade(1);
                        break;
                    }
                    case Keyboard::Num3: {
                        player.upgrade(2);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        limpaVetor(projeteis, inimigos, drops);
        if (!pauseflag) {
            updater(player, projeteis, inimigos, drops, base, textos);
            if (tempoinimigo.asMilliseconds() > 2500) {
                spawnaInimigos(inimigos, randomizer, &projeteis, &player, &base, &drops);
                tempoinimigo = milliseconds(0);
            }
            if(timerjogo.getElapsedTime().asSeconds() > 1) {
                timeelapsed += timerjogo.getElapsedTime();
                timerjogo.restart();
            }
            tempoinimigo += timerinimigo.getElapsedTime();
            timerinimigo.restart();
        }
        else {
            textos.update();
        }
        window.clear();
        window.draw(base.sprite);
        window.draw(player.sprite);
        for (Projetil & proj : projeteis) {
            window.draw(proj.sprite);
        }
        for (Inimigo & inimigo : inimigos) {
            window.draw(inimigo.sprite);
        }
        for (Loot & loot : drops) {
            window.draw(loot.sprite);
        }
        window.draw(textos.relogio);
        window.draw(textos.muni);
        window.draw(textos.hpplayer);
        window.draw(textos.hpbase);
        window.draw(textos.dinheiro);
        window.draw(textos.upgrades);
        if(loseflag){
            window.draw(textos.perdeu);
            pauseflag = true;
        }
        if(timeelapsed > maxgametime) {
            window.draw(textos.ganhou);
            pauseflag = true;
        }
        window.display();
    }
}