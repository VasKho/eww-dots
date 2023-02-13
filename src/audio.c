#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

#define BUFF_SIZE 10
#define ICON_SIZE ( strlen("婢")+1 )


void mute_toggle() {
  FILE* pipe = popen("pactl get-sink-mute @DEFAULT_SINK@", "r");
  char buff[BUFF_SIZE], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  if (strncmp(buff, "Mute: yes", 9) == 0) {
    system("pactl set-sink-mute @DEFAULT_SINK@ 0");
  } else {
    system("pactl set-sink-mute @DEFAULT_SINK@ 1");
  }
  pclose(pipe);
}

int get_volume_value() {
  FILE* pipe = popen("echo $(pactl get-sink-volume @DEFAULT_SINK@) | grep -Eo '[0-9]{1,3}%' | head -1", "r");
  if (pipe == NULL) {
    return -1;
  }
  char buff[4], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  pclose(pipe);
  buff[--i] = '\0';
  return atoi(buff);
}

int update_volume_value(char* direction, int by_value) {
  int cur_value = get_volume_value();
  int new_value = -1;
  if (strcmp(direction, "down") == 0) {
    if (cur_value <= 100-by_value) {
      new_value = cur_value + by_value;
    } else {
      new_value = 100;
    }
  } else if (strcmp(direction, "up") == 0) {
    if (cur_value >= by_value) {
      new_value = cur_value - by_value;
    } else {
      new_value = 0;
    }
  }
  char tmp[] = "pactl set-sink-volume @DEFAULT_SINK@ ";
  char* str_value = calloc(4, sizeof(char));
  snprintf(str_value, 4, "%d", new_value);
  char* command = calloc(strlen(tmp)+strlen(str_value)+2, sizeof(char));
  strcat(command, tmp);
  strcat(command, str_value);
  free(str_value);
  strcat(command, "%");
  system(command);
  free(command);
  return new_value;
}

int get_icon(int value, char* save_buff) {
  FILE* pipe = popen("pactl get-sink-mute @DEFAULT_SINK@", "r"); 
  if (pipe == NULL) { 
    return -1; 
  } 
  char buff[BUFF_SIZE], tmp; 
  size_t i = 0; 
  while ((tmp = fgetc(pipe)) != EOF) { 
    buff[i++] = tmp; 
  } 
  pclose(pipe); 
  char icon[ICON_SIZE];
  if (strncmp(buff, "Mute: yes", 9) == 0) {
    memcpy(icon, "婢", ICON_SIZE);
    eww_update_variable("volume-color", "#fb4934");
    eww_update_variable("volume-value", icon); 
    exit(0); 
  } else { 
    if (70 < value && value <= 100) {
      memcpy(icon, "墳", ICON_SIZE);
    } else if (30 < value && value <= 70) {
      memcpy(icon, "奔", ICON_SIZE);
    } else if (0 <= value && value <= 30) {
      memcpy(icon, "奄", ICON_SIZE);
    } 
  }
  memcpy(save_buff, icon, ICON_SIZE);
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    if (strncmp(argv[1], "toggle", 6) == 0) {
      mute_toggle();
    }
  } else if (argc == 3) {
    update_volume_value(argv[1], atoi(argv[2]));
  }
  int value = get_volume_value();
  char icon[ICON_SIZE];
  get_icon(value, icon);
  eww_update_variable("volume-color", "$font-col");
  char tmp_value[5];
  snprintf(tmp_value, 5, "%d%%", value);
  char* out_value = calloc(strlen(icon)+strlen(tmp_value)+2, sizeof(char));
  strcat(out_value, icon);
  strcat(out_value, " ");
  strcat(out_value, tmp_value);
  eww_update_variable("volume-value", out_value);
  free(out_value);
  return 0;
}
