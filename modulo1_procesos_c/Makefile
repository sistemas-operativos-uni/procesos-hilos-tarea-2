CC = gcc
CFLAGS = -Wall -Wextra -O2
 
SRC_DIR = src
BIN_DIR = bin
 
all: $(BIN_DIR)/mostrador $(BIN_DIR)/preparar_pedido
 
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
 
$(BIN_DIR)/mostrador: $(SRC_DIR)/mostrador.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<
 
$(BIN_DIR)/preparar_pedido: $(SRC_DIR)/preparar_pedido.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<
 
clean:
	rm -rf $(BIN_DIR)
 
.PHONY: all clean