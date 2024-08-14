#include "Inimigo.hpp"
#include "Funcoes.hpp"

Inimigo::Inimigo(float angulo, float delay, std::vector<Projetil> *proj, Player *jogador, Base *bas, int money, int bullet, std::vector<Loot> *pickup){
    playeraddr = jogador;
    projeteisaddr = proj;
    lootaddr = pickup;
    baseaddr = bas;
    dinheiro = money;
    muni = bullet;
    Vector2f temp = {escalarcentrotela * (float)cos(angulo * M_PI), escalarcentrotela * (float)sin(angulo * M_PI)};
    temp *= delay;
    temp += centrotela;
    sprite.setPosition(temp);
    sprite.setRadius(raiosprite);
    sprite.setFillColor(Color::White);
    sprite.setOutlineThickness(2);
    sprite.setOutlineColor(Color::Red);
    sprite.setOrigin(raiosprite, raiosprite);
    alvo = centrotela - temp;
    versoralvo = calculaVersor(alvo);
    velocidade = versoralvo * movespeed;
}

void Inimigo::update(){
    sprite.move(velocidade);

    if (elapsed.asMilliseconds() > 1500){
        atirar();
        elapsed = milliseconds(0);
    }
    for (Projetil &projetil : *projeteisaddr){
        if (projetil.time == 0){
            if (sprite.getGlobalBounds().intersects(projetil.sprite.getGlobalBounds())){
                spawnarloot();
                clrflag = true;
                projetil.clrflag = true;
            }
        }
    }
    if (sprite.getGlobalBounds().intersects(baseaddr->sprite.getGlobalBounds())){
        if (!clrflag){
            baseaddr->levardano();
        }
        clrflag = true;
    }
    elapsed += shoottimer.getElapsedTime();
    shoottimer.restart();
}

void Inimigo::atirar(){
    projeteisaddr->emplace_back(playeraddr->sprite.getPosition(), sprite.getPosition(), 1);
}

void Inimigo::spawnarloot(){
    lootaddr->emplace_back(dinheiro, muni, sprite.getPosition());
}

void Inimigo::pausado(){
    elapsed += shoottimer.getElapsedTime();
}

void Inimigo::despausado(){
    shoottimer.restart();
}