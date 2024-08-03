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
const float raioprojetil = 10.f;
//pra 1080p, 960x540
const Vector2f centrotela = {800, 450};
const Vector2f dimensoestela = {1600, 900};
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)); //nota para si: não esqueça da raiz quadrada do escalar
}
Vector2f calculaVersor(Vector2f vetor) { //Não sei se vale a pena essa
    return vetor/calculaEscalar(vetor);
}
class Bola{ //classe criadas por motivos de herança pra não ter que repetir um monte de vezes esses
public:
    float movespeed;
    CircleShape sprite;
    Vector2f velocidade;
    Vector2f alvo;
    Vector2f versoralvo; //e foi no exato momento que percebi que eu ia precisar de versor pra fazer isso de uma forma bacana
    //que eu me senti genial por fazer VGA
};
class Projetil : public Bola {
    float movespeed = 10;
    int time; //será utilizado depois para propósitos de colisão; 0 = player, 1 = inimigo
public:
    bool clrflag = false;
    Projetil(Vector2f target, Vector2f pos, int team){
        versoralvo = calculaVersor(target-pos);
        velocidade = versoralvo*movespeed;
        sprite.setPosition(pos);
        sprite.setRadius(raioprojetil);
        sprite.setFillColor(Color::Red);
        sprite.setOrigin(raioprojetil, raioprojetil);
        time = team;
    };
    void update(){
        sprite.move(velocidade);
        if (sprite.getPosition().x < 0 || sprite.getPosition().x > dimensoestela.x || sprite.getPosition().y < 0 || sprite.getPosition().y > dimensoestela.y) {
            clrflag = true;
        }
    };
};
class Shooter : public Bola{
protected:
    vector<Projetil>* projeteisaddr;
};
class Player : public Shooter{
    int hp;
    int dinheiro;
    int muni;
public:
    Window *janela;
    Player(float posX, float posY, Window *vindow, vector<Projetil> *proj){
        janela = vindow;
        projeteisaddr = proj;
        hp = 100;
        muni = 100;
        dinheiro = 0;
        velocidade = {0,0};
        versoralvo = {0,0};
        movespeed = 10;
        sprite.setPosition(posX, posY);
        sprite.setRadius(raiosprite);
        sprite.setOutlineColor(Color::Green);
        sprite.setOutlineThickness(2);
        sprite.setFillColor(Color::White);
        sprite.setOrigin(raiosprite, raiosprite);
        alvo = {0,0};
    }
    void update(){
        if(Mouse::isButtonPressed(sf::Mouse::Right)) {
            alvo = (Vector2f)Mouse::getPosition(*janela) - sprite.getPosition(); //casting pra V2f por motivos de o Mouse::getPosition retornar V2i
        }
        calculoMovimento();
        sprite.move(velocidade);
    }
    void atirar(){
        projeteisaddr->emplace_back((Vector2f)Mouse::getPosition(*janela), sprite.getPosition(), 0);
        //aparentemente nas versões mais novas do C++ tem uma função pra vetor chamada emplace_back que faz o mesmo que push_back([typename](argumentos))
    }
    void calculoMovimento(){//Joguei o código pra dentro duma função pra ficar mais fácil de ler
        if ((alvo.x != 0) || (alvo.y != 0)) {
            versoralvo = alvo / calculaEscalar(alvo);
            velocidade = versoralvo * movespeed;
            if (abs(velocidade.x) > abs(alvo.x)) { velocidade.x = alvo.x;}
            if (abs(velocidade.y) > abs(alvo.y)) { velocidade.y = alvo.y;}
            alvo -= velocidade;
        }
        else { velocidade = {0,0};}
    }
    void levardano(){
    }
    float x() { return sprite.getPosition().x; };
    float y() { return sprite.getPosition().y; };
    float cima() { return y() - sprite.getPosition().y / 2.f; };
    float direita() { return x() + sprite.getPosition().x / 2.f; };
    float baixo() { return y() + sprite.getPosition().y / 2.f; };
    float esquerda() { return x() - sprite.getPosition().x / 2.f; };
};
class Inimigo : public Shooter{
    Inimigo(){};
    void update(){
        sprite.move(velocidade);
    }
    void morrer(){
    };
};
class Base{
    int hp;
private:
    void levardano(){};
public:
    RectangleShape sprite;
    void update(){
        //colisão vai aqui
    }
    Base(){
        sprite.setFillColor(Color::Black);
        sprite.setOutlineColor(Color::Blue);
        sprite.setOutlineThickness(10);
        sprite.setSize({250,100});
        sprite.setPosition(centrotela);
        sprite.setOrigin({125,50});
    }
};




int main() {

    auto window = sf::RenderWindow{ { 1600u, 900u }, "Amo videogames"};
    //algum de vocês por favor busque depois fazer com que se você botar maximizado ele ainda vá pra onde o mouse está
    //provavelmente vai requerer catucar com o sf::Event::Resized
    window.setFramerateLimit(60 );
    window.setKeyRepeatEnabled(false);
    vector<Projetil> projeteis;
    Player player(centrotela.x, centrotela.y, &window, &projeteis);
    Base base;
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if(event.key.code == Keyboard::Q) {
                    player.atirar();
                }
            }
        }
        player.update();
        for (Projetil & proj : projeteis) {
            proj.update();
        }
        projeteis.erase(std::remove_if(projeteis.begin(), projeteis.end(), [&](const Projetil &item) {
            return item.clrflag;
        }), projeteis.end()); //levemente gambiarrento mas pelo que entendi o remove if mata o elemento e joga pro final do vetor
        //e o erase apaga
        window.clear();
        window.draw(base.sprite);
        window.draw(player.sprite);
        for (Projetil & proj : projeteis) {
            window.draw(proj.sprite);
        }
        window.display();
    }
}
