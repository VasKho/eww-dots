#include <stdio.h>
#include <stdlib.h>
#include "eww_utils.h"

#ifndef BINARY_OUTPUT
#define BINARY_OUTPUT "./"
#endif

void run_script(char* path_prefix, char* binary_name) {
  char* command = calloc(strlen(path_prefix)+strlen(binary_name)+1, sizeof(char));
  strcat(command, path_prefix);
  strcat(command, binary_name);
  system(command);
  free(command);
}

int launch_eww() {
  if (eww_is_running()) {
    return 1;
  } else {
    eww_run_daemon();
    int pid = fork();
    if (pid == 0) {
      char* prefix = calloc(strlen(BINARY_OUTPUT)+2, sizeof(char));
      memcpy(prefix, BINARY_OUTPUT, strlen(BINARY_OUTPUT));
      memcpy(prefix+strlen(BINARY_OUTPUT), "/", strlen("/"));
      run_script(prefix, "audio");
      run_script(prefix, "backlight");
      run_script(prefix, "mic");
      free(prefix);
    } else {
      eww_open_window("bar");
    }
  }
  return -1;
}

int close_eww() {
  if (eww_is_running()) {
    system("killall eww");
    return 0;
  }
  return 1;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return 0;
  } else if (argc == 2) {
    if (strcmp(argv[1], "start") == 0) {
      if (launch_eww() == 1) printf("Eww is already running.\n");
    } else if (strcmp(argv[1], "stop") == 0) {
      if (close_eww() == 1) printf("Eww is not running.\n");
    } else if (strcmp(argv[1], "restart") == 0) {
      close_eww();
      launch_eww();
    }
  }
  return 0;
}
