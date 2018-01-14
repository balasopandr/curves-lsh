CC= gcc
CFLAGS= -c -g -Wall
LDFLAGS=-lm -llapacke -llapack -lopenblas
BIN:=../bin
SRC:=./

SOURCES:= $(wildcard $(SRC)/*.c)

SRC_OBJECTS=$(SOURCES:.c=.o)
OBJECTS=$(SRC_OBJECTS:$(SRC)%=$(BIN)%)
OUT=$(BIN)/conformations

all: $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BIN)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(wildcard $(BIN)/*)

