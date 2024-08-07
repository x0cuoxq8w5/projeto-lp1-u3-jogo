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

const float raiosprite = 16.f; // Raio do sprite do jogador e inimigos
const float raioprojetil = 10.f; // Raio do sprite do projetil
const Vector2f centrotela = {800, 450}; // Coordenadas do centro da tela para resolução de 1600x900 - Artur
const Vector2f dimensoestela = {1600, 900}; // Dimensões da tela
const string wintext = "Você venceu!";
const string losetext = "Perdeu!";
const Time maxgametime = seconds(180.f);


// Função para calcular a magnitude (escalar) de um vetor 2D
float calculaEscalar(Vector2f vetor) {
    return sqrt((vetor.x*vetor.x)+(vetor.y*vetor.y)); //nota para si: não esqueça da raiz quadrada do escalar - Artur
}

const float escalarcentrotela = calculaEscalar(centrotela);

// Função para calcular o versor (vetor unitário) de um vetor 2D
Vector2f calculaVersor(Vector2f vetor) { //Não sei se vale a pena esse - Artur
    return vetor/calculaEscalar(vetor);
}

// Classe base Bola, usada para herança em outras classes
class Bola{
public:
    float movespeed{}; // Velocidade de movimento
    CircleShape sprite; // Sprite do objeto
    Vector2f velocidade; // Vetor de velocidade
    Vector2f alvo; // Posição alvo
    Vector2f versoralvo; // Versor do vetor alvo
};

// Classe Projetil, derivada de Bola
class Projetil : public Bola {
    float movespeed = 10; // Velocidade do projetil
public:
    int time; // Será utilizado depois para propósitos de colisão; 0 = player, 1 = inimigo - Artur
    bool clrflag = false;

    // Construtor do projetil
    Projetil(Vector2f target, Vector2f pos, int team){
        if(target-pos == Vector2f{0,0}) {
            target.x++;
            target.y++;
        }
        versoralvo = calculaVersor(target-pos); // Calcula o versor do alvo
        velocidade = versoralvo*movespeed; // Define a velocidade baseada no versor e na velocidade
        sprite.setPosition(pos); // Define a posição do sprite
        sprite.setRadius(raioprojetil); // Define o raio do sprite
        sprite.setFillColor(Color::Yellow); // Define a cor do sprite
        sprite.setOrigin(raioprojetil, raioprojetil); // Define a origem do sprite
        time = team; // Define o time do projetil
    };

    // Função de atualização do projetil
    void update(){
        sprite.move(velocidade); // Move o sprite baseado na velocidade
        // Se o projetil sair da tela, ativa a flag de limpeza
        if (sprite.getPosition().x < 0 || sprite.getPosition().x > dimensoestela.x || sprite.getPosition().y < 0 || sprite.getPosition().y > dimensoestela.y) {
            clrflag = true;
        }
    };
};

// Classe Loot (itens dropados)
class Loot{
public:
    RectangleShape sprite{{40,30}}; // Sprite do loot com tamanho 40x30
    int dinheiro, muni; // Quantidade de dinheiro e munição no loot
    Clock despawntimer; // Timer para o despawn do loot
    bool clrflag = false;

    // Construtor do loot
    Loot(int cash, int ammo, Vector2f pos){
        dinheiro = cash;
        muni = ammo;
        sprite.setOrigin(sprite.getSize().x/2, sprite.getSize().y/2); // Define a origem do sprite
        sprite.setPosition(pos); // Define a posição do sprite
        sprite.setFillColor(Color::Yellow); // Define a cor do sprite
    };
    void update(){}; // Função de atualização vazia (por enquanto)
};

// Classe Shooter, derivada de Bola
class Shooter : public Bola{
protected:
    vector<Projetil>* projeteisaddr{}; // Ponteiro para vetor de projeteis
    vector<Loot>* lootaddr{}; // Ponteiro para vetor de loots
};

// Classe Player, derivada de Shooter
class Player : public Shooter{
public:
    int hp = 100, dinheiro = 0, muni = 25;
    Window *janela;

    // Construtor do jogador
    Player(float posX, float posY, Window *vindow, vector<Projetil> *proj, vector<Loot>* drops){
        janela = vindow; // Inicializa o ponteiro da janela
        projeteisaddr = proj; // Inicializa o ponteiro dos projeteis
        lootaddr = drops; // Inicializa o ponteiro dos loots
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

    // Função de atualização do jogador
    void update(){
	    // Se o botão direito do mouse for pressionado, define o alvo
        if(Mouse::isButtonPressed(sf::Mouse::Right)) {
            alvo = (Vector2f)Mouse::getPosition(*janela) - sprite.getPosition();
            //casting pra V2f por motivos de o Mouse::getPosition retornar V2i - Artur
        }
        calculoMovimento();
        calculoColisao();
        sprite.move(velocidade);
    }

    // Função para o jogador atirar
    void atirar(){
        if (muni > 0) {
            projeteisaddr->emplace_back((Vector2f) Mouse::getPosition(*janela), sprite.getPosition(), 0);
            muni--;
        }
        //aparentemente nas versões mais novas do C++ tem uma função pra vetor chamada emplace_back que faz o mesmo que push_back([vetor](argumentos))
        // - Artur
    }

    // Função para calcular o movimento do jogador
    void calculoMovimento(){//Joguei o código pra dentro duma função pra ficar mais fácil de ler - Artur
        if ((alvo.x != 0) || (alvo.y != 0)) {
            versoralvo = alvo / calculaEscalar(alvo);
            velocidade = versoralvo * movespeed;
            if (abs(velocidade.x) > abs(alvo.x)) { velocidade.x = alvo.x;} // Ajusta a velocidade no eixo x
            if (abs(velocidade.y) > abs(alvo.y)) { velocidade.y = alvo.y;} // Ajusta a velocidade no eixo y
            alvo -= velocidade; // Atualiza o alvo
        }
        else { velocidade = {0,0};} // Se não há alvo, a velocidade é zero
    }

    // Função para calcular a colisão do jogador
    void calculoColisao(){
	// Verifica colisão com projeteis
        for (Projetil & i : *projeteisaddr) {
            if (i.time == 1) {
                if (sprite.getGlobalBounds().intersects(i.sprite.getGlobalBounds())) {
                    i.clrflag = true;
                    hp--;
                }
            }
        }
	// Verifica colisão com loots
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

// Classe Inimigo, derivada de Shooter
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