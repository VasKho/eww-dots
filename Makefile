CC := gcc
CFLAGS := -DEWW_CONFIG_DIRECTORY=\"$(shell pwd)\"

SRC_DIR := $(shell pwd)/src
BIN_DIR := $(shell pwd)/bin

WS_SRC := $(SRC_DIR)/sway-ipc.c $(SRC_DIR)/workspaces.c

all: workspaces

workspaces: $(WS_SRC)
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) $(WS_SRC) -o $(BIN_DIR)/workspaces -ljansson

clean:
	rm -rf $(BIN_DIR)
