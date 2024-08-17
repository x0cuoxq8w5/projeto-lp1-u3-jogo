# Instruções para Compilar e Executar o Jogo

## Requisitos de Sistema

Antes de começar, certifique-se de que o sistema possui os seguintes requisitos instalados:
- **g++**: Um compilador C++.
- **SFML**: Simple and Fast Multimedia Library (SFML) para gráficos, janela e sistema. Certifique-se de ter as bibliotecas `sfml-graphics`, `sfml-window` e `sfml-system` instaladas.

## Clonar ou Baixar o Projeto

Baixe ou clone o repositório do projeto para o seu sistema:

```bash
git clone <URL do repositório>
```

## Compilar o Jogo
Compile o jogo utilizando o Makefile fornecido. No terminal, execute o seguinte comando:

```bash
make
```
Este comando compilará todos os arquivos de código-fonte e gerará um executável chamado jogo.

Ou execute o seguinte comando:

```bash
g++ -I./include ./src/Globals.cpp ./src/Bola.cpp ./src/Projetil.cpp ./src/Loot.cpp ./src/Base.cpp ./src/Shooter.cpp ./src/Player.cpp ./src/Inimigo.cpp ./src/GameText.cpp ./src/Funcoes.cpp ./src/main.cpp -o jogo -lsfml-graphics -lsfml-window -lsfml-system
```

Caso não tenha o SFML instalado (por estar no windows, ou caso os outros metodos não tenham funcionado), utilize o CMake com o comando
```shell
cmake -G "MinGW Makefiles" -B build -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
```
Note: Dependendo de sua toolchain, você precisará especificar parâmetros diferentes para as flags -G, -D CMAKE_C_COMPILER, e -D CMAKE_CXX_COMPILER

Isto criará uma pasta "./build/" que será utilizada para compilar o programa
Em seguida, no mesmo diretório que você uitilizou o comando anterior, utilize o comando 
```shell
cmake --build build --target main
```
Para criar o arquivo "main.exe" na pasta ./build/bin

## Executar o Jogo
Após a compilação, você pode executar o jogo com o comando:

```bash
./jogo
```
Se preferir, você pode usar a regra run do Makefile:

```bash
make run
```
Note que o jogo não terá elementos de UI caso a pasta do executável não possua um arquivo "arial.ttf" (como o já incluso na pasta bin do cmake-release), por não ter carregado a fonte.

## Controles
Movimento é realizado clicando com o botão direito do mouse na posição onde quer chegar \
Disparo de projéteis é realizado com a tecla Q \
Jogo pode ser pausado com ESC

## Limpar os Arquivos Compilados (Opcional)
Se desejar remover o executável e outros arquivos gerados pela compilação, execute:

```bash
make clean
```
