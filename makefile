# Diretórios
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin

# Arquivos e extensões
EXEC := $(BIN_DIR)/pacman
SRC := $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp) ./main.cpp
OBJ := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.cpp=.o))
OBJ := $(patsubst ./%, $(OBJ_DIR)/%, $(OBJ))
OBJ := $(filter-out $(OBJ_DIR)/main.o, $(OBJ)) $(OBJ_DIR)/main/main.o
DEP := $(OBJ:.o=.d)

# Compilador e flags
CXX := g++
CXXFLAGS := -Wall -Wextra -I$(INC_DIR) -std=c++17 -MMD -MP
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Regras principais
all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Trata especificamente main.cpp para ser compilado em ./obj/main/main.o
$(OBJ_DIR)/main/main.o: ./main.cpp
	@mkdir -p $(OBJ_DIR)/main
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(OBJ_DIR)/*/*.o $(OBJ_DIR)/*/*.d $(BIN_DIR)/*

# Limpa apenas objetos
clean-obj:
	rm -rf $(OBJ_DIR)

# Executa o programa
run: $(EXEC)
	$(EXEC)

# Recompila tudo do zero
rebuild: clean all

# Inclui dependências
-include $(DEP)
