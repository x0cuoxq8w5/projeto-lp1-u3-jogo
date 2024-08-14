# Nome do executável
TARGET = jogo

# Diretórios de código-fonte e include
SRC_DIR = ./src
INC_DIR = ./include

# Arquivos de código-fonte
SRCS = $(SRC_DIR)/Globals.cpp \
       $(SRC_DIR)/Bola.cpp \
       $(SRC_DIR)/Projetil.cpp \
       $(SRC_DIR)/Loot.cpp \
       $(SRC_DIR)/Base.cpp \
       $(SRC_DIR)/Shooter.cpp \
       $(SRC_DIR)/Player.cpp \
       $(SRC_DIR)/Inimigo.cpp \
       $(SRC_DIR)/GameText.cpp \
       $(SRC_DIR)/Funcoes.cpp \
       $(SRC_DIR)/main.cpp

# Flags do compilador
CXX = g++
CXXFLAGS = -I$(INC_DIR) -lsfml-graphics -lsfml-window -lsfml-system

# Regras do Makefile
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean run