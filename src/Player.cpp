#include "Player.hpp"
#include "Funcoes.hpp"
#include <iostream>

Player::Player(float posX, float posY, Window *vindow, vector<Projetil> *proj, vector<Loot> *drops, Base *base1, bool *loseflag){
    janela = vindow;
    projeteisaddr = proj;
    lootaddr = drops;
    baseaddr = base1;
    loseflagaddr = loseflag;
    velocidade = {0, 0};
    versoralvo = {0, 0};
    movespeed = 8;

    sprite.setPosition(posX, posY);
    sprite.setRadius(raiosprite);
    sprite.setOutlineColor(Color::Green);
    sprite.setOutlineThickness(2);
    sprite.setFillColor(Color::White);
    sprite.setOrigin(raiosprite, raiosprite);
    alvo = {0, 0};
}

void Player::update(){
    if (Mouse::isButtonPressed(sf::Mouse::Right)){
        alvo = (Vector2f)Mouse::getPosition(*janela) - sprite.getPosition();
    }
    if (hp == 0){
        *loseflagaddr = true;
    }
    calculoMovimento();
    calculoColisao();
    sprite.move(velocidade);
}

void Player::atirar(){
    if (muni > 0){
        projeteisaddr->emplace_back((Vector2f)Mouse::getPosition(*janela), sprite.getPosition(), 0, shotspeed);
        muni--;
    }
}

void Player::calculoMovimento(){
    if ((alvo.x != 0) || (alvo.y != 0)){
        versoralvo = alvo / calculaEscalar(alvo);
        velocidade = versoralvo * movespeed;
        if (abs(velocidade.x) > abs(alvo.x)){
            velocidade.x = alvo.x;
        } // Ajusta a velocidade no eixo x
        if (abs(velocidade.y) > abs(alvo.y)){
            velocidade.y = alvo.y;
        } // Ajusta a velocidade no eixo y
        alvo -= velocidade;
    }
    else{
        velocidade = {0, 0};
    }
}

void Player::calculoColisao(){
    // Verifica colisão com projeteis
    for (Projetil & i : *projeteisaddr){
        if (i.time == 1){
            if (sprite.getGlobalBounds().intersects(i.sprite.getGlobalBounds())){
                i.clrflag = true;
                hp--;
            }
        }
    }
    // Verifica colisão com loots
    for (Loot & i : *lootaddr){
        if (sprite.getGlobalBounds().intersects(i.sprite.getGlobalBounds()))
        {
            i.clrflag = true;
            muni += i.muni;
            dinheiro += i.dinheiro;
        }
    }
};

void Player::upgrade(int qual){
    if (dinheiro >= precos[qual])
    {
        switch (qual){
            case 0:
                movespeed++;
                break;
            case 1:
                shotspeed++;
                break;
            case 2:
                baseaddr->regen++;
                break;
            default:
                cout << "ALGO DEU MUITO ERRADO SE ISSO PRINTOU" << endl;
                break;
        }
        dinheiro -= precos[qual];
        precos[qual] = floor((float)precos[qual] * 1.5);
    }
}
