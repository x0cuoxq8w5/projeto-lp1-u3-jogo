#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;
// Ideia pra conseguir o resto da pontuação: Implementar um menu de upgrade acessível apertando ESC
// Nesse menu você poderia talvez aumentar o seu hp máximo, hp máximo da base e botar pra base regenerar - Artur
const float raiosprite = 16.f;
const float raioprojetil = 10.f;
const Vector2f centrotela = {800, 450}; //pra 1080p, 960x540 - Artur
const Vector2f dimensoestela = {1600, 900};
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)); //nota para si: não esqueça da raiz quadrada do escalar - Artur
}
const float escalarcentrotela = calculaEscalar(centrotela);
Vector2f calculaVersor(Vector2f vetor) { //Não sei se vale a pena esse - Artur
    return vetor/calculaEscalar(vetor);
}
class Bola{ //classe criadas por motivos de herança pra não ter que repetir um monte de vezes esses - Artur
public:
    float movespeed;
    CircleShape sprite;
    Vector2f velocidade;
    Vector2f alvo;
    Vector2f versoralvo; //e foi no exato momento que percebi que eu ia precisar de versor pra fazer isso de uma forma bacana
    //que eu me senti genial por fazer VGA - Artur
};
class Projetil : public Bola {
    float movespeed = 10;
    int time; //será utilizado depois para propósitos de colisão; 0 = player, 1 = inimigo - Artur
public:
    bool clrflag = false;
    Projetil(Vector2f target, Vector2f pos, int team){
        versoralvo = calculaVersor(target-pos);
        velocidade = versoralvo*movespeed;
        sprite.setPosition(pos);
        sprite.setRadius(raioprojetil);
        sprite.setFillColor(Color::Yellow);
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
    int hp, dinheiro, muni;
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
            alvo = (Vector2f)Mouse::getPosition(*janela) - sprite.getPosition();
            //casting pra V2f por motivos de o Mouse::getPosition retornar V2i - Artur
        }
        calculoMovimento();
        sprite.move(velocidade);
    }
    void atirar(){
        projeteisaddr->emplace_back((Vector2f)Mouse::getPosition(*janela), sprite.getPosition(), 0);
        //aparentemente nas versões mais novas do C++ tem uma função pra vetor chamada emplace_back que faz o mesmo que push_back([vetor](argumentos))
        // - Artur
    }
    void calculoMovimento(){//Joguei o código pra dentro duma função pra ficar mais fácil de ler - Artur
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
    float movespeed = 5;
    Clock shoottimer;
    Player *player;
public:
    bool clrflag = false;
    Inimigo(float angulo, float delay, vector<Projetil> *proj, Player *jogador){
        player = jogador;
        projeteisaddr = proj;
        Vector2f temp = {escalarcentrotela*(float)cos(angulo*M_PI),escalarcentrotela*(float)sin(angulo*M_PI)};
        temp *= delay;
        temp += centrotela;
        sprite.setPosition(temp);
        sprite.setRadius(raiosprite);
        sprite.setFillColor(Color::White);
        sprite.setOutlineThickness(2);
        sprite.setOutlineColor(Color::Red);
        sprite.setOrigin(raiosprite, raiosprite);
        alvo = centrotela-temp;
        versoralvo = calculaVersor(alvo);
        velocidade = versoralvo*movespeed;
    };
    void update(){
        sprite.move(velocidade);
        if(shoottimer.getElapsedTime().asMilliseconds() > 1500) {
            atirar();
            shoottimer.restart();
        }
    }
    void atirar(){
        projeteisaddr->emplace_back(player->sprite.getPosition(), sprite.getPosition(), 1);
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
        //colisão vai aqui - Artur
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
class Loot{
public:
    RectangleShape sprite;
    int hp, dinheiro, muni;
    Clock despawntimer;
    bool clrflag;
    Loot(){};
    void update(){};
};

void updater(Player &player, vector<Projetil> &projeteis, vector<Inimigo> &inimigos, vector<Loot> &drops, Base &base) {
    player.update();
    for (Projetil & proj : projeteis) {
        proj.update();
    }
    for (Inimigo & inimigo : inimigos) {
        inimigo.update();
    }
    for (Loot & loot : drops) {
        loot.update();
    }
    base.update();
}
void limpaVetor(vector<Projetil> &projeteis, vector<Inimigo> &inimigos, vector<Loot> &drops){
    projeteis.erase(std::remove_if(projeteis.begin(), projeteis.end(), [&](const Projetil &item) {
        return item.clrflag;
    }), projeteis.end()); //levemente gambiarrento mas pelo que entendi o remove if mata o elemento e joga pro final do vetor
    //e o erase apaga - Artur
    inimigos.erase(std::remove_if(inimigos.begin(), inimigos.end(), [&](const Inimigo &item) {
        return item.clrflag;
    }), inimigos.end());
    drops.erase(std::remove_if(drops.begin(), drops.end(), [&](const Loot &item) {
        return item.clrflag;
    }), drops.end());
}
void spawnaInimigos(vector<Inimigo> &inimigos, mt19937 &rand, vector<Projetil> *addrprojeteis, Player* player) {
    uniform_int_distribution<int> quantia(2,4);
    uniform_real_distribution<float> onde(0.f,2.f);
    uniform_real_distribution<float> slow(0.9f,1.6f);
    int qtd = quantia(rand);
    double angulo, delay;
    for (int i = 0; i < qtd; ++i) {
        angulo = onde(rand);
        delay = slow(rand);
        inimigos.emplace_back(angulo, delay, addrprojeteis, player);
    }
}

int main() {
    auto window = sf::RenderWindow{ { 1600u, 900u }, "Amo videogames"};
    //algum de vocês por favor busque depois fazer com que se você botar maximizado ele ainda vá pra onde o mouse está
    //provavelmente vai requerer catucar com o sf::Event::Resized - Artur
    window.setFramerateLimit(60 );
    vector<Projetil> projeteis;
    vector<Inimigo> inimigos;
    vector<Loot> drops;
    Player player(centrotela.x, centrotela.y, &window, &projeteis);
    Base base;
    Clock timerinimigo, timerjogo;
    random_device isca;
    mt19937 randomizer(isca());

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
        updater(player, projeteis, inimigos, drops, base);
        limpaVetor(projeteis, inimigos, drops);
        if (timerinimigo.getElapsedTime().asMilliseconds() > 2500) {
            spawnaInimigos(inimigos, randomizer, &projeteis, &player);
            timerinimigo.restart();
        }
        window.clear();
        window.draw(base.sprite);
        window.draw(player.sprite);
        for (Projetil & proj : projeteis) {
            window.draw(proj.sprite);
        }
        for (Inimigo & inimigo : inimigos) {
            window.draw(inimigo.sprite);
        }
        for (Loot & loot : drops) {
            window.draw(loot.sprite);
        }
        window.draw(base.sprite);
        window.display();
    }
}
