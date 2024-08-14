// 1

#ifndef BOLA_HPP
#define BOLA_HPP

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class Bola {
public:
    float movespeed{}; // Velocidade de movimento
    CircleShape sprite; // Sprite do objeto
    Vector2f velocidade; // Vetor de velocidade
    Vector2f alvo; // Posição alvo
    Vector2f versoralvo; // Versor do vetor alvo
};

#endif