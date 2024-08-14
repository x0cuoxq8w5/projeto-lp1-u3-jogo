// 0

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

const float raiosprite = 16.f; // Raio do sprite do jogador e inimigos
const float raioprojetil = 10.f; // Raio do sprite do projetil
const Vector2f dimensoestela = {1600, 900}; // Dimensões da tela
const Vector2f centrotela = {dimensoestela.x/2,dimensoestela.y/2};
const string wintext = "Você venceu!";
const string losetext = "Perdeu!";
const Time maxgametime = seconds(180.f);

float calculaEscalar(Vector2f vetor);
Vector2f calculaVersor(Vector2f vetor);

const float escalarcentrotela = calculaEscalar(centrotela);

#endif // GLOBALS_HPP
