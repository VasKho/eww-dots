#!/usr/bin/env bash

set -eo pipefail

INSTALLER_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)

set -o allexport
if [ -f "${INSTALLER_PATH}/.env" ]; then
    source "${INSTALLER_PATH}/.env"
else
    source "${INSTALLER_PATH}/.env.example"
fi
set +o allexport

BUILD_DIR="${INSTALLER_PATH}/build"

__help() {
    cat <<USAGE
Usage: $0 [OPTION]

If no option is specified then this message is printed.

OPTIONS:
    all			Run all installation stages
    check	        Check for installed dependencies
    clean		Remove build and install dir
    help		Print this message and exit
    install		Copy build to installation directory
    prepare	        Run preinstall config instructions
USAGE
    exit 1
}

___workspaces() {
    CFLAGS=("-DEWW_CONFIG_DIRECTORY=\"${EWW_INSTALL_DIR}\"")
    SRC_DIR="${BUILD_DIR}/eww/src"
    WS_SRC=("${SRC_DIR}/sway-ipc.c" "${SRC_DIR}/workspaces.c")
    BIN_DIR="${BUILD_DIR}/eww/bin"

    [[ ! -d "${BIN_DIR}" ]] && mkdir "${BIN_DIR}"
    gcc ${CFLAGS[*]} ${WS_SRC[*]} -o "${BIN_DIR}/workspaces" -ljansson
}

__prepare() {
    EWW_CONFIG_SRC=(./bar ./calendar *.scss *.yuck ./powermenu ./screenshot ./scripts ./src)

    mkdir -p "${BUILD_DIR}/eww"
    cp -r ${EWW_CONFIG_SRC[*]} "${BUILD_DIR}/eww"
    for file in $(find ${EWW_CONFIG_SRC[*]} -type f); do
	envsubst '${EWW_INSTALL_DIR}' <"${file}" > "${BUILD_DIR}/eww/${file}"
    done
    ___workspaces
}

__install() {
    [[ -d "${EWW_INSTALL_DIR}" ]] && echo "${EWW_INSTALL_DIR} already exists" && exit 1
    mkdir -p "${EWW_INSTALL_DIR}" && cp -r "${BUILD_DIR}/eww/"* "${EWW_INSTALL_DIR}"
}

__check() {
    EXECUTABLES=("eww" "wpctl" "brightnessctl" "nmcli")

    for exec in "${EXECUTABLES[@]}"; do
	which $exec > /dev/null 2>/dev/null
	if [[ $? -eq 1 ]]; then
	    echo "Unable to find ${exec}"
	    exit 1
	fi
    done
}

__clean() {
    rm -rf ${BUILD_DIR}
    rm -rf ${EWW_INSTALL_DIR}
}

__all() {
    __check
    __prepare
    __install
}

declare -F "__${1}" >/dev/null && "__${1}" || echo "Invalid command ${1}. Aborting."; exit 1
