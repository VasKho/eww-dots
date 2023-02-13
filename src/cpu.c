#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

int get_value() {
  FILE* pipe = popen("awk '{u=$2+$4; t=$2+$4+$5; if (NR==1){u1=u; t1=t;} else printf \"%.1f\", ($2+$4-u1) * 100 / (t-t1); }' <(grep 'cpu ' /proc/stat) <(sleep 1;grep 'cpu ' /proc/stat)", "r");
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
    eww_update_variable("cpu-animation", "animation: blink-critical 1.5s linear 0s infinite alternate;");
  } else if (value >= 70) {
    eww_update_variable("cpu-animation", "animation: blink-warning 1.5s linear 0s infinite alternate;");
  } else {
    eww_update_variable("cpu-animation", "${hov_select} 0.443s linear 0s 1 normal;");
  }
  return 0;
}
