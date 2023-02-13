#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

#define BUFF_SIZE 13

int get_network_status() {
  FILE* pipe = popen("nmcli dev status | grep -E \"wifi\" -m 1 | awk '{print ($3)}'", "r");
  if (pipe == NULL) {
    return -1;
  }
  char buff[BUFF_SIZE], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  pclose(pipe);
  return (strncmp(buff, "disconnected", 12) == 0)? 0 : 1;
}

int main() {
  int status = get_network_status();
  if (status) {
    printf("直");
    eww_update_variable("wifi-color", "");
  } else {
    printf("睊");
    eww_update_variable("wifi-color", "#fb4934");
  }
  return 0;
}
