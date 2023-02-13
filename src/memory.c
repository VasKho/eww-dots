#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

int get_value() {
  FILE* pipe = popen("printf \"%.0f\n\" $(free -m | grep Mem | awk '{print ($3/$2)*100}')", "r");
  if (pipe == NULL) {
    return -1;
  }
  char buff[4], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  pclose(pipe);
  return atoi(buff);
}

int main() {
  int value = get_value();
  printf("%d\n", value);
  if (value >= 90) {
    eww_update_variable("memory-animation", "animation: blink-critical 1.5s linear 0s infinite alternate;");
  } else if (value >= 70) {
    eww_update_variable("memory-animation", "animation: blink-warning 1.5s linear 0s infinite alternate;");
  } else {
    eww_update_variable("memory-animation", "${hov_select} 0.443s linear 0s 1 normal;");
  }
  return 0;
}
