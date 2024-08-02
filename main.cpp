#include <iostream>
#include <string>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;
// Ideia pra conseguir o resto da pontuação: Implementar um menu de upgrade acessível apertando ESC
// Nesse menu você poderia talvez aumentar o seu hp máximo, hp máximo da base e botar pra base regenerar
const float raiosprite = 16.f;
//pra 1080p, 960x540
const Vector2f centrotela = {800, 450};
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)); //nota para si: não esqueça da raiz quadrada do escalar
}

class Player {
    int hp;
    int dinheiro;
    int muni;
public:
    Window *janela;
    float movespeed;
    CircleShape sprite;
    Vector2f velocidade;
    Vector2f alvo;
    Vector2f versoralvo; //e foi no exato momento que percebi que eu ia precisar de versor pra fazer isso de uma forma bacana
    //que eu me senti genial por fazer VGA
    Player(float posX, float posY, Window *vindow){
        janela = vindow;
        hp = 100;
        muni = 100;
        dinheiro = 0;
        velocidade = {0,0};
        versoralvo = {0,0};
        movespeed = 10;
        sprite.setPosition(posX, posY);
        sprite.setRadius(raiosprite);
        sprite.setOutlineColor(Color::Cyan);
        sprite.setOutlineThickness(4);
        sprite.setFillColor(Color::White);
        sprite.setOrigin(raiosprite, raiosprite);
        alvo = sprite.getPosition();
    }
    void update(){
        if(Mouse::isButtonPressed(sf::Mouse::Right)) {
            alvo = (Vector2f)Mouse::getPosition(*janela) - sprite.getPosition(); //casting pra V2f por motivos de o Mouse::getPosition retornar V2i
        }
        if ((alvo.x != 0) && (alvo.y != 0)) {
            versoralvo = alvo / calculaEscalar(alvo);
            velocidade = versoralvo * movespeed;
            if (calculaEscalar(alvo) < calculaEscalar(velocidade)) {
                velocidade = alvo;
            }
            alvo -= velocidade;
        }
        else { velocidade = {0,0};}
        /*cout << "Spritepos: " << sprite.getPosition().x << ", " << sprite.getPosition().y << "\nAlvo: " << alvo.x << ", "
        << alvo.y << "\nVelocidade: " << velocidade.x << ", " << velocidade.y << endl;;*/
        sprite.move(velocidade);
    }
    void atirar(){
    }
    void levardano(){
    }
    float x() {return sprite.getPosition().x;};
    float y() {return sprite.getPosition().y;};
    float cima() {return y() - sprite.getPosition().y/2.f;};
    float direita() {return x() + sprite.getPosition().x/2.f;};
    float baixo() {return y() + sprite.getPosition().y/2.f;};
    float esquerda() {return x() - sprite.getPosition().x/2.f;};
};
class Inimigo{
    CircleShape sprite;
    void morrer(){

    };
};




int main() {

    auto window = sf::RenderWindow{ { 1600u, 900u }, "Amo videogames"};
    //algum de vocês por favor busque depois fazer com que se você botar maximizado ele ainda vá pra onde o mouse está
    //provavelmente vai requerer catucar com o sf::Event::Resized
    window.setFramerateLimit(60 );
    Player player(centrotela.x, centrotela.y, &window);
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        player.update();
        window.clear();
        window.draw(player.sprite);
        window.display();
    }
}
