#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

#define ICON_SIZE ( strlen("")+1 )

int battery() {
  FILE* pipe = popen("ls /sys/class/power_supply | grep BAT | head -n 1", "r");
  char buff[5], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  buff[i-1] = '\0';
  pclose (pipe);
  char* temp = calloc(strlen("/sys/class/power_supply/")+strlen(buff)+strlen("/capacity")+1, sizeof(char));
  strcat(temp, "/sys/class/power_supply/");
  strcat(temp, buff);
  strcat(temp, "/capacity");
  FILE* f = fopen(temp, "r");
  if (f == NULL) {
    return -1;
  }
  free(temp);
  int res = 0;
  fscanf(f, "%d", &res);
  fclose(f);
  return res;
}

char* battery_status() {
  FILE* pipe = popen("ls /sys/class/power_supply | grep BAT | head -n 1", "r");
  char buff[5], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  buff[i-1] = '\0';
  pclose (pipe);
  char* temp = calloc(strlen("/sys/class/power_supply/")+strlen(buff)+strlen("/status")+1, sizeof(char));
  strcat(temp, "/sys/class/power_supply/");
  strcat(temp, buff);
  strcat(temp, "/status");
  FILE* f = fopen(temp, "r");
  if (f == NULL) {
    return "";
  }
  free(temp);
  char* res = calloc(strlen("Discharging")+1, sizeof(char));
  fscanf(f, "%s", res);
  fclose(f);
  return res;
}

int get_icon(int value, char* status, char* save_buff) {
  char icon[ICON_SIZE];
  if (strcmp(status, "Discharging") == 0) {
    if (80 < value && value <= 100) {
      memcpy(icon, "", ICON_SIZE);
    } else if (60 < value && value <= 80) {
      memcpy(icon, "", ICON_SIZE);
    } else if (40 < value && value <= 60) {
      memcpy(icon, "", ICON_SIZE);
    } else if (20 < value && value <= 40) {
      memcpy(icon, "", ICON_SIZE);
    } else if (10 < value && value <= 20) {
      memcpy(icon, "", ICON_SIZE);
    } else if (0 <= value && value <= 20) {
      memcpy(icon, "", ICON_SIZE);
    }
  } else {
    if (80 < value && value <= 100) {
      memcpy(icon, "", ICON_SIZE);
    } else if (60 < value && value <= 80) {
      memcpy(icon, "", ICON_SIZE);
    } else if (40 < value && value <= 60) {
      memcpy(icon, "", ICON_SIZE);
    } else if (20 < value && value <= 40) {
      memcpy(icon, "", ICON_SIZE);
    } else if (10 < value && value <= 20) {
      memcpy(icon, "", ICON_SIZE);
    } else if (0 <= value && value <= 10) {
      memcpy(icon, "", ICON_SIZE);
    }
  }
  memcpy(save_buff, icon, ICON_SIZE);
  return 0;
}

int main() {
  int value = battery();
  char* status = battery_status();
  char icon[ICON_SIZE];
  get_icon(value, status, icon);
  free(status);
  if (value <= 10) {
    eww_update_variable("battery-animation", "animation: blink-critical 1.5s linear 0s infinite alternate;");
  } else if (value <= 30) {
    eww_update_variable("battery-animation", "animation: blink-warning 1.5s linear 0s infinite alternate;");
  } else {
    eww_update_variable("battery-animation", "none;");
  }
  char tmp_value[5];
  snprintf(tmp_value, 5, "%d%%", value);
  char* out_value = calloc(strlen(icon)+strlen(tmp_value)+2, sizeof(char));
  strcat(out_value, icon);
  strcat(out_value, " ");
  strcat(out_value, tmp_value);
  printf("%s", out_value);
  free(out_value);
  return 0;
}
