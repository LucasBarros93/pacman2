# Diretórios
SRC_DIR := source
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Arquivos e extensões
EXEC := $(BIN_DIR)/pacman
SRC := $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.cpp=.o))
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

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

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
