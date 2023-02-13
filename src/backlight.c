#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eww_utils.h"

#define ICON_SIZE ( strlen("")+1 )

int get_backlight_value() {
  FILE* pipe = popen("brightnessctl g", "r");
  if (pipe == NULL) {
    return -1;
  }
  char buff[4], tmp;
  size_t i = 0;
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  pclose(pipe);
  int current = atoi(buff);
  pipe = popen("brightnessctl m", "r");
  if (pipe == NULL) {
    return -1;
  }
  i = 0;
  bzero(buff, 3);
  while ((tmp = fgetc(pipe)) != EOF) {
    buff[i++] = tmp;
  }
  pclose(pipe);
  int max = atoi(buff);
  return 100*current/max;
}

int update_backlight_value(char* direction, int by_value) { 
  int cur_value = get_backlight_value(); 
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
  char tmp[] = "brightnessctl s "; 
  char* str_value = calloc(4, sizeof(char)); 
  snprintf(str_value, 4, "%d", new_value); 
  char* command = calloc(strlen(tmp)+strlen(str_value)+strlen(" > /dev/null")+2, sizeof(char)); 
  strcat(command, tmp);
  strcat(command, str_value); 
  free(str_value); 
  strcat(command, "%");
  strcat(command, " > /dev/null");
  system(command); 
  free(command); 
  return new_value; 
} 

int get_icon(int value, char* save_buff) {
  char* icon[ICON_SIZE];
  if (70 < value && value <= 100) {
    memcpy(icon, "", ICON_SIZE);
  } else if (50 < value && value <= 70) {
    memcpy(icon, "", ICON_SIZE);
  } else if (40 < value && value <= 50) {
    memcpy(icon, "", ICON_SIZE);
  } else if (30 < value && value <= 40) {
    memcpy(icon, "", ICON_SIZE);
  } else if (20 < value && value <= 30) {
    memcpy(icon, "", ICON_SIZE);
  } else if (10 < value && value <= 20) {
    memcpy(icon, "", ICON_SIZE);
  } else if (0 <= value && value <= 10) {
    memcpy(icon, "", ICON_SIZE);
  }
  memcpy(save_buff, icon, ICON_SIZE);
  return 0;
} 

int main(int argc, char* argv[]) {
  if (argc == 3) {
    update_backlight_value(argv[1], atoi(argv[2]));
  }
  int value = get_backlight_value();
  char icon[ICON_SIZE];
  get_icon(value, icon);
  char tmp_value[5];
  snprintf(tmp_value, 5, "%d%%", value);
  char* out_value = calloc(strlen(icon)+strlen(tmp_value)+2, sizeof(char));
  strcat(out_value, icon);
  strcat(out_value, " ");
  strcat(out_value, tmp_value);
  eww_update_variable("backlight-value", out_value);
  free(out_value);
  return 0;
}
