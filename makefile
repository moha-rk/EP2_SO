COMPILER = gcc

all: race

SRC_DIR = ./src
LIBS_DIR = ./libs
SIM_FLAGS = -Wall -I$(LIBS_DIR) -lpthread

LIBS_NAMES = ciclist.h race.h
LIBS = $(patsubst %,$(LIBS_DIR)/%,$(LIBS_NAMES))

OBJ_NAMES = ciclist.o race.o simulation.o
OBJ = $(patsubst %,$(SRC_DIR)/%,$(OBJ_NAMES))

$(SRC_DIR)/%.o: %.c $(LIBS)
	$(COMPILER) -c -o $@ $< $(SIM_FLAGS)

race: $(OBJ)
	$(COMPILER) -o $@ $^ $(SIM_FLAGS)

clean:
	rm -f $(shell find . -name '*.o' -or -name 'race')