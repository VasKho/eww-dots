#ifndef _EWW_UTILS_H_
#define _EWW_UTILS_H_

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EWW_CONFIG_DIRECTORY
#define EWW_CONFIG_DIRECTORY "./"
#endif

int eww_is_running() {
  FILE* pipe = popen("pidof eww", "r");
  if (pipe == NULL) {
    return -1;
  }
  int pid = 0;
  fread(&pid, sizeof(pid), 1, pipe);
  pclose(pipe);
  return pid? 1 : 0;
}

void eww_run_daemon() {
  char* command = calloc(strlen("eww -c ")+strlen(EWW_CONFIG_DIRECTORY)+strlen(" daemon")+1, sizeof(char));
  strcat(command, "eww -c ");
  strcat(command, EWW_CONFIG_DIRECTORY);
  strcat(command, " daemon");
  system(command);
  free(command);
}

void eww_open_window(char* name) {
  char* command = calloc(strlen("eww -c ")+strlen(EWW_CONFIG_DIRECTORY)+strlen(" open-many ")+strlen(name)+1, sizeof(char));
  strcat(command, "eww -c ");
  strcat(command, EWW_CONFIG_DIRECTORY);
  strcat(command, " open-many ");
  strcat(command, name);
  system(command);
  free(command);
}

void eww_update_variable(char* name, char* new_value) {
  char* name_value = calloc(strlen(name)+strlen(new_value)+2, sizeof(char));
  strcat(name_value, name);
  strcat(name_value, "=");
  strcat(name_value, new_value);
  int pid = fork(), status;
  if (pid == 0) {
    execlp("eww", "eww", "-c", EWW_CONFIG_DIRECTORY, "update", name_value, NULL);
  } else {
    waitpid(pid, &status, 0);
    free(name_value);
  }
}

#endif
