#include "Funcoes.hpp"

void updater(Player &player, vector<Projetil> &projeteis, vector<Inimigo> &inimigos, vector<Loot> &drops, Base &base, GameText &textos) {
    player.update();
    for (Projetil & proj : projeteis) {
        proj.update();
    }
    for (Loot & loot : drops) {
        loot.update();
    }
    for (Inimigo & inimigo : inimigos) {
        inimigo.update();
    }
    base.update();
    textos.update();
}

void limpaVetor(vector<Projetil> &projeteis, vector<Inimigo> &inimigos, vector<Loot> &drops){
    projeteis.erase(std::remove_if(projeteis.begin(), projeteis.end(), [&](const Projetil &item) {
        return item.clrflag;
    }), projeteis.end());
    inimigos.erase(std::remove_if(inimigos.begin(), inimigos.end(), [&](const Inimigo &item) {
        return item.clrflag;
    }), inimigos.end());
    drops.erase(std::remove_if(drops.begin(), drops.end(), [&](const Loot &item) {
        return item.clrflag;
    }), drops.end());
}

void spawnaInimigos(vector<Inimigo> &inimigos, mt19937 &rand, vector<Projetil> *addrprojeteis, Player* player, Base* base, vector<Loot>* drops) {
    std::uniform_int_distribution<int> quantia(2,4), grana(1,3), balas(0,5);
    std::uniform_real_distribution<float> onde(0.f,2.f), slow(0.9f,1.3f);
    int qtd = quantia(rand), dinheiro, muni;
    double angulo, delay;
    for (int i = 0; i < qtd; ++i) {
        dinheiro = grana(rand);
        muni = balas(rand);
        angulo = onde(rand);
        delay = slow(rand);
        inimigos.emplace_back(angulo, delay, addrprojeteis, player, base, dinheiro, muni, drops);
    }
}

void pausador(vector<Inimigo> &inimigos, vector<Loot> &drops, Base &base){
    for (Inimigo & i : inimigos) {
        i.pausado();
    }
    for (Loot & i : drops) {
        i.pausado();
    }
    base.pausado();
}

void despausador(vector<Inimigo> &inimigos, vector<Loot> &drops, Base &base){
    for (Inimigo & i : inimigos) {
        i.despausado();
    }
    for (Loot & i : drops) {
        i.despausado();
    }
    base.despausado();
}