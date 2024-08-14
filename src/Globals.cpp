#include "Globals.hpp"

// Função para calcular a magnitude (escalar) de um vetor 2D
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y));
}

// Função para calcular o versor (vetor unitário) de um vetor 2D
Vector2f calculaVersor(Vector2f vetor) {
    return vetor/calculaEscalar(vetor);
}