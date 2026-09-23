# Directories
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin

# Files and extentions
EXEC := $(BIN_DIR)/pacman
SRC := $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.cpp=.o))
DEP := $(OBJ:.o=.d)

# Compilator and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -I$(INC_DIR) -std=c++17 -MMD -MP
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Main rules
all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(OBJ_DIR)/*/*.o $(OBJ_DIR)/*/*.d $(BIN_DIR)/*

run: $(EXEC)
	$(EXEC)

rebuild: clean all

# Dependencies
-include $(DEP)
