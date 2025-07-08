CC = g++
CPPFLAGS = -std=c++11 -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
BIN = $(BIN_DIR)/tp3.out

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) 
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(BIN)

$(BIN): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

rm:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.out