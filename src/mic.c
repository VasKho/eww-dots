#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

#define BUFF_SIZE 10

void mute_toggle() {
  FILE* pipe = popen("pactl get-source-mute @DEFAULT_SOURCE@", "r");
  char buff[BUFF_SIZE], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  if (strncmp(buff, "Mute: yes", 9) == 0) {
    system("pactl set-source-mute @DEFAULT_SOURCE@ 0");
    eww_update_variable("micro-rev", "true");
  } else {
    system("pactl set-source-mute @DEFAULT_SOURCE@ 1");
    eww_update_variable("micro-rev", "false");
  }
  pclose(pipe);
}

int main() {
  mute_toggle();
  return 0;
}
