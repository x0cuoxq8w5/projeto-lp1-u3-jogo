#include "GameText.hpp"
#include "Globals.hpp"

GameText::GameText(Player *player1, Base *base1, Time *tempojogo){
    fonte.loadFromFile("arial.ttf");
    
    player = player1;
    base = base1;
    tempojogoatual = tempojogo;

    relogio.setPosition(0, 0);
    muni.setPosition(0, fonte.getLineSpacing(muni.getCharacterSize()));
    hpplayer.setPosition(0, fonte.getLineSpacing(muni.getCharacterSize()) * 2);
    hpbase.setPosition(0, fonte.getLineSpacing(muni.getCharacterSize()) * 3);
    dinheiro.setPosition(0, fonte.getLineSpacing(muni.getCharacterSize()) * 4);
    upgrades.setPosition(0, dimensoestela.y - fonte.getLineSpacing(muni.getCharacterSize()) - 2);
    ganhou.setPosition(centrotela);
    //ganhou.setOrigin(ganhou.getGlobalBounds().getSize() / 2.f + ganhou.getLocalBounds().getPosition());
    ganhou.setOrigin((ganhou.getGlobalBounds().width / 2.f) + ganhou.getLocalBounds().left, (ganhou.getGlobalBounds().height / 2.f) + ganhou.getLocalBounds().top);
    perdeu.setPosition(centrotela);
    //perdeu.setOrigin(perdeu.getGlobalBounds().getSize() / 2.f + perdeu.getLocalBounds().getPosition());
    perdeu.setOrigin((perdeu.getGlobalBounds().width / 2.f) + perdeu.getLocalBounds().left, (perdeu.getGlobalBounds().height / 2.f) + perdeu.getLocalBounds().top);
}

void GameText::update(){
    calculaRelogio();
    muni.setString("Muni:" + to_string(player->muni));
    hpplayer.setString("Vida:" + to_string(player->hp));
    hpbase.setString("Base:" + to_string(base->hp));
    dinheiro.setString("Dinheiro:" + to_string(player->dinheiro));
    upgrades.setString("[1] Movespeed: " + to_string(player->precos[0]) + " [2] Shotspeed: " + to_string(player->precos[1]) + " [3] Base regen: " + to_string(player->precos[2]));
}

void GameText::calculaRelogio(){
    int mins = ((int)maxgametime.asSeconds() - (int)tempojogoatual->asSeconds()) / 60;
    int secs = ((int)maxgametime.asSeconds() - (int)tempojogoatual->asSeconds()) % 60;
    string segs;
    if (secs < 10){
        segs = "0";
        segs.append(to_string(secs));
    }
    else{
        segs = to_string(secs);
    }
    string texto = to_string(mins) + ":" + segs;
    relogio.setString(texto);
}
