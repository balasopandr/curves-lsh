CC= gcc
CFLAGS= -c -g -Wall
LDFLAGS=-lm
BIN:=../bin_tests
SRC:=../src

METRICS=$(BIN)/test

SOURCES = curve.c curve_metrics.c data_parser.c helper_functions.c list.c pair_int.c complete_binary_tree.c free_memory.c hash_table.c hash_data.c vector.c cluster.c
OBJECTS = $(SOURCES:.c=.o)
BIN_OBJECTS = $(OBJECTS:%=$(BIN)/%)

all: $(METRICS)

$(METRICS): $(BIN_OBJECTS) $(BIN)/curve_metrics_test.o $(BIN)/main.o $(BIN)/mean_curve.o
	$(CC) $(BIN_OBJECTS) $(BIN)/curve_metrics_test.o $(BIN)/main.o $(BIN)/mean_curve.o -o $(METRICS) $(LDFLAGS)

$(BIN)/curve_metrics_test.o: curve_metrics_test.c
	$(CC) $(CFLAGS) curve_metrics_test.c -o $(BIN)/curve_metrics_test.o

$(BIN)/main.o: main.c
	$(CC) $(CFLAGS) main.c -o $(BIN)/main.o

$(BIN)/mean_curve.o: mean_curve.c
	$(CC) $(CFLAGS) mean_curve.c -o $(BIN)/mean_curve.o

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(wildcard $(BIN)/*)
