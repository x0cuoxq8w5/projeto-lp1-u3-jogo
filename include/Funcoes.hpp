// 9

#ifndef FUNCOES_HPP
#define FUNCOES_HPP

#include "Player.hpp"
#include "Inimigo.hpp"
#include "GameText.hpp"
#include <random>

using namespace std;

void updater(Player &player, vector<Projetil> &Projetil, vector<Inimigo> &inimigos, vector<Loot> &drops, Base &base, GameText &textos);
void limpaVetor(vector<Projetil> &projeteis, vector<Inimigo> &inimigos, vector<Loot> &drops);
void spawnaInimigos(vector<Inimigo> &inimigos, mt19937 &rand, vector<Projetil> *addrprojeteis, Player* player, Base* base, vector<Loot>* drops);
void pausador(vector<Inimigo> &inimigos, vector<Loot> &drops, Base &Base, vector<Projetil> &projeteis);
void despausador(vector<Inimigo> &inimigos, vector<Loot> &drops, Base &Base, vector<Projetil> &projeteis);

#endif
