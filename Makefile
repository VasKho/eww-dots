CC := gcc
CFLAGS := -DEWW_CONFIG_DIRECTORY=\"$(shell pwd)\"

SRC_DIR := $(shell pwd)/src
BIN_DIR := $(shell pwd)/bin

WS_SRC := $(SRC_DIR)/sway-ipc.c $(SRC_DIR)/workspaces.c
MODES_SRC := $(SRC_DIR)/sway-ipc.c $(SRC_DIR)/modes.c

all: audio backlight battery cpu memory modes launch mic wifi workspaces

audio: ./src/audio.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/audio.c -o $(BIN_DIR)/audio 

backlight: ./src/backlight.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/backlight.c -o $(BIN_DIR)/backlight

battery: ./src/battery.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/battery.c -o $(BIN_DIR)/battery

cpu: ./src/cpu.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/cpu.c -o $(BIN_DIR)/cpu

memory: ./src/memory.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/memory.c -o $(BIN_DIR)/memory

modes: $(MODES_SRC)
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) $(MODES_SRC) -o $(BIN_DIR)/modes -ljansson

launch: ./src/launch.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) -DBINARY_OUTPUT=\"$(BIN_DIR)\" ./src/launch.c -o $(BIN_DIR)/launch

mic: ./src/mic.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/mic.c -o $(BIN_DIR)/mic

wifi: ./src/wifi.c
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) ./src/wifi.c -o $(BIN_DIR)/wifi

workspaces: $(WS_SRC)
	$(shell [[ ! -d $(BIN_DIR) ]] && mkdir $(BIN_DIR))
	$(CC) $(CFLAGS) $(WS_SRC) -o $(BIN_DIR)/workspaces -ljansson

clean:
	rm -rf $(BIN_DIR)
