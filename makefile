COMPILER = gcc

all: race

BCCSH_DIR = ./shell
SHELL_FLAGS = -Wall -lreadline
SHELL_OBJ = ./shell/bccsh.o

$(BCCSH_DIR)/%.o: %.c
	$(COMPILER) -c -o $@ $^ $(SHELL_FLAGS)

bccsh: $(SHELL_OBJ)
	$(COMPILER) -o $@ $^ $(SHELL_FLAGS)

SRC_DIR = ./simulador/src
LIBS_DIR = ./simulador/libs
SIM_FLAGS = -Wall -I$(LIBS_DIR) -lpthread

LIBS_NAMES = fila.h process.h simulador.h
LIBS = $(patsubst %,$(LIBS_DIR)/%,$(LIBS_NAMES))

OBJ_NAMES = escalonador.o simulador.o fila.o process.o
OBJ = $(patsubst %,$(SRC_DIR)/%,$(OBJ_NAMES))

$(SRC_DIR)/%.o: %.c $(LIBS)
	$(COMPILER) -c -o $@ $< $(SIM_FLAGS)

ep1: $(OBJ)
	$(COMPILER) -o $@ $^ $(SIM_FLAGS)

clean:
	rm -f $(shell find . -name '*.o' -or -name 'race' -or -name 'ep1')