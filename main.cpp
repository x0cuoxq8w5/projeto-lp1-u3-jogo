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
const string wintext = "Você venceu!"; //Fique atento: std::string != sf::string - Artur
const string losetext = "Perdeu!";
const Time maxgametime = seconds(180.f);
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)); //nota para si: não esqueça da raiz quadrada do escalar - Artur
}
const float escalarcentrotela = calculaEscalar(centrotela);
Vector2f calculaVersor(Vector2f vetor) { //Não sei se vale a pena esse - Artur
    return vetor/calculaEscalar(vetor);
}
class Bola{ //classe criadas por motivos de herança pra não ter que repetir um monte de vezes esses - Artur
public:
    float movespeed{};
    CircleShape sprite;
    Vector2f velocidade;
    Vector2f alvo;
    Vector2f versoralvo; //e foi no exato momento que percebi que eu ia precisar de versor pra fazer isso de uma forma bacana
    //que eu me senti genial por fazer VGA - Artur
};
class Projetil : public Bola {
    float movespeed = 10;
public:
    int time; //será utilizado depois para propósitos de colisão; 0 = player, 1 = inimigo - Artur
    bool clrflag = false;
    Projetil(Vector2f target, Vector2f pos, int team){
        if(target-pos == Vector2f{0,0}) {
            target.x++;
            target.y++;
        }
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
class Loot{
public:
    RectangleShape sprite{{40,30}};
    int dinheiro, muni;
    Clock despawntimer;
    bool clrflag = false;
    Loot(int cash, int ammo, Vector2f pos){
        dinheiro = cash;
        muni = ammo;
        sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
        sprite.setPosition(pos);
        sprite.setFillColor(Color::Yellow);
    };
    void update(){};
};
class Shooter : public Bola{
protected:
    vector<Projetil>* projeteisaddr{};
    vector<Loot>* lootaddr{};
};
class Player : public Shooter{
public:
    int hp = 100, dinheiro = 0, muni = 25;
    Window *janela;
    Player(float posX, float posY, Window *vindow, vector<Projetil> *proj, vector<Loot>* drops){
        janela = vindow;
        projeteisaddr = proj;
        lootaddr = drops;
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
        calculoColisao();
        sprite.move(velocidade);
    }
    void atirar(){
        if (muni > 0) {
            projeteisaddr->emplace_back((Vector2f) Mouse::getPosition(*janela), sprite.getPosition(), 0);
            muni--;
        }
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
    void calculoColisao(){
        for (Projetil & i : *projeteisaddr) {
            if (i.time == 1) {
                if (sprite.getGlobalBounds().intersects(i.sprite.getGlobalBounds())) {
                    i.clrflag = true;
                    hp--;
                }
            }
        }
        for (Loot & i : *lootaddr) {
            if (sprite.getGlobalBounds().intersects(i.sprite.getGlobalBounds())) {
                i.clrflag = true;
                muni += i.muni;
                dinheiro += i.dinheiro;
            }
        }
    };
    void levardano(){
    }
};
class Base{
public:
    int hp = 100;
    RectangleShape sprite{{275,125}};
    void update(){
        if (hp == 0) {
            perdeu();
        }
    }
    void levardano() {
        hp--;
    }
    Base(){
        sprite.setFillColor(Color(127,127,192));
        sprite.setPosition(centrotela);
        sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2);
    }
    void perdeu() {

    };
};
class Inimigo : public Shooter{
    float movespeed = 5;
    Clock shoottimer;
    Player *playeraddr;
    Base *baseaddr;
    int dinheiro, muni;
public:
    bool clrflag = false;
    Inimigo(float angulo, float delay, vector<Projetil> *proj, Player *jogador, Base *bas, int money, int bullet, vector<Loot>* pickup){
        playeraddr = jogador;
        projeteisaddr = proj;
        lootaddr = pickup;
        baseaddr = bas;
        dinheiro = money;
        muni = bullet;
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
        for (Projetil & projetil : *projeteisaddr) {
            if (projetil.time == 0) {
                if (sprite.getGlobalBounds().intersects(projetil.sprite.getGlobalBounds())) {
                    spawnarloot();
                    clrflag = true;
                    projetil.clrflag = true;
                }
            }
            if (sprite.getGlobalBounds().intersects(baseaddr->sprite.getGlobalBounds())) {
                clrflag = true;
                baseaddr->levardano();
            }
        }
    }
    void atirar(){
        projeteisaddr->emplace_back(playeraddr->sprite.getPosition(), sprite.getPosition(), 1);
    }
    void spawnarloot(){
        lootaddr->emplace_back(dinheiro, muni, sprite.getPosition());
    };
};
class GameText{
    Font fonte;
    Player* player;
    Base* base;
    Clock *tempojogoatual;
public:
    Text hpplayer{"DEFAULT", fonte, 30}, hpbase{"DEFAULT", fonte, 30},
    relogio{"DEFAULT", fonte, 30}, muni{"DEFAULT", fonte, 30},
    dinheiro{"DEFAULT", fonte, 30};
    GameText(Player *player1, Base* base1, Clock* relogiojogo){
        fonte.loadFromFile("arial.ttf");
        /*Isso não é um bom jeito de carregar a fonte pois requer que você
        tenha na mesma pasta que o EXE o arquivo de fonte. Algum de vocês veja de achar uma solução melhor, isso é
        gambiarrento e isso me deixa triste; - Artur*/
        player = player1;
        base = base1;
        tempojogoatual = relogiojogo;
        relogio.setPosition(0,0);
        muni.setPosition(0, fonte.getLineSpacing(30));
        hpplayer.setPosition(0, fonte.getLineSpacing(30)*2);
        hpbase.setPosition(0, fonte.getLineSpacing(30)*3);
        dinheiro.setPosition(0,fonte.getLineSpacing(30)*4);
    }
    void update(){
        calculaRelogio();
        muni.setString("Muni:" + to_string(player->muni));
        hpplayer.setString("Vida:" + to_string(player->hp));
        hpbase.setString("Base:" + to_string(base->hp));
        dinheiro.setString("Dinheiro:" + to_string(player->dinheiro));
    }
    void calculaRelogio(){
        int mins = ((int)maxgametime.asSeconds()-(int)tempojogoatual->getElapsedTime().asSeconds())/60;
        int secs = ((int)maxgametime.asSeconds()-(int)tempojogoatual->getElapsedTime().asSeconds())%60;
        string segs;
        if (secs<10) {
            segs = "0";
            segs.append(to_string(secs));
        }
        else {
            segs = to_string(secs);
        }
        string texto = to_string(mins) + ":" + segs;
        relogio.setString(texto);
    }
};

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
    }), projeteis.end()); //levemente gambiarrento mas pelo que entendi o remove if mata o elemento e joga pro final do vetor
    //e o erase apaga - Artur
    inimigos.erase(std::remove_if(inimigos.begin(), inimigos.end(), [&](const Inimigo &item) {
        return item.clrflag;
    }), inimigos.end());
    drops.erase(std::remove_if(drops.begin(), drops.end(), [&](const Loot &item) {
        return item.clrflag;
    }), drops.end());
}
void spawnaInimigos(vector<Inimigo> &inimigos, mt19937 &rand, vector<Projetil> *addrprojeteis, Player* player, Base* base, vector<Loot>* drops) {
    uniform_int_distribution<int> quantia(2,4), grana(1,6), balas(0,10);
    uniform_real_distribution<float> onde(0.f,2.f), slow(0.9f,1.3f);
    //Note: "onde" é utilizado pra determinar o ângulo do inimigo em relação ao centro, e "slow" determina um """delay""" a mais
    //a propósito de evitar que todos venham exatamente ao mesmo tempo
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

int main() {
    auto window = sf::RenderWindow{ { 1600u, 900u }, "Amo videogames"};
    //algum de vocês por favor busque depois fazer com que se você botar maximizado ele ainda vá pra onde o mouse está
    //provavelmente vai requerer catucar com o sf::Event::Resized - Artur
    window.setFramerateLimit(60 );
    vector<Projetil> projeteis;
    vector<Inimigo> inimigos;
    vector<Loot> drops;
    Player player(centrotela.x, centrotela.y, &window, &projeteis, &drops);
    Base base;
    Clock timerinimigo, timerjogo;
    random_device isca;
    mt19937 randomizer(isca());
    GameText textos(&player, &base, &timerjogo);
    bool pauseflag = false;
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
        limpaVetor(projeteis, inimigos, drops);
        updater(player, projeteis, inimigos, drops, base, textos);
        if (timerinimigo.getElapsedTime().asMilliseconds() > 2500) {
            spawnaInimigos(inimigos, randomizer, &projeteis, &player, &base, &drops);
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
        window.draw(textos.relogio);
        window.draw(textos.muni);
        window.draw(textos.hpplayer);
        window.draw(textos.hpbase);
        window.display();
    }
}
