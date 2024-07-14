MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
EWW_INSTALL_DIR := ${HOME}/.config/eww
EXECUTABLES := eww wpctl brightnessctl nmcli
ROW := "%-15s|%-20s\n"

EWW_CONFIG_SRC := ./bar ./calendar *.scss *.yuck ./powermenu ./screenshot ./scripts ./src
BUILD_DIR = $(MAKEFILE_DIR)build

CC := gcc
CFLAGS := -DEWW_CONFIG_DIRECTORY=\"$(EWW_INSTALL_DIR)\"

SRC_DIR := $(BUILD_DIR)/eww/src
BIN_DIR := $(BUILD_DIR)/eww/bin

WS_SRC := $(SRC_DIR)/sway-ipc.c $(SRC_DIR)/workspaces.c


all: prepare workspaces install

prepare:
	mkdir -p $(BUILD_DIR)/eww && cp -r $(EWW_CONFIG_SRC) $(BUILD_DIR)/eww
	find $(BUILD_DIR)/eww -type f -exec sed -i 's|EWW_INSTALL_DIR|$(EWW_INSTALL_DIR)|gm' {} \;


workspaces: $(WS_SRC)
	@if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
	$(CC) $(CFLAGS) $(WS_SRC) -o $(BIN_DIR)/workspaces -ljansson

install:
	@if [ -d $(EWW_INSTALL_DIR) ]; then echo "$(EWW_INSTALL_DIR) already exists"; exit 1; fi
	mkdir -p $(EWW_INSTALL_DIR) && cp -r $(BUILD_DIR)/eww/* $(EWW_INSTALL_DIR)

check:
	for exec in $(EXECUTABLES); do which $$exec > /dev/null 2>/dev/null && printf $(ROW) "$$exec" "Found" || printf $(ROW) "$$exec" "Not found"; done


clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(EWW_INSTALL_DIR)

.PHONY: check
.SILENT: check
