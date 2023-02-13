#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "sway-ipc.h"
#include "eww_utils.h"

void concatenate(char** dest, char* src) {
  *dest = realloc(*dest, strlen(*dest)+strlen(src)+1);
  strncat(*dest, src, strlen(src));
}

void get_ws_info(ipc_response* resp) {
  const char* out_str = "(box :class \"workspaces\" :space-evenly false :orientation \"h\" :vexpand true";
  char* output = calloc(strlen(out_str)+1, sizeof(char));
  strcpy(output, out_str);
  FILE* pipe = popen("swaymsg -t get_workspaces", "r");
  json_t* ws_info = json_loadf(pipe, 0, NULL);
  size_t ind = 0;
  json_t* value;
  char* num_buff = calloc(3, sizeof(char));
  json_array_foreach(ws_info, ind, value) {
    char* button = calloc(strlen(" (button")+1, sizeof(char));
    strcpy(button, " (button");
    void* iter = json_object_iter_at(value, "name");
    json_t* name = json_object_iter_value(iter);
    iter = json_object_iter_at(value, "focused");
    json_t* focused = json_object_iter_value(iter);
    if (json_boolean_value(focused)) {
      concatenate(&button, " :class \"ws_button_focused\"");
    } else {
      concatenate(&button, " :class \"ws_button\"");
    }
    concatenate(&button, " :onclick \"swaymsg workspace number ");
    iter = json_object_iter_at(value, "num");
    json_t* num = json_object_iter_value(iter);
    snprintf(num_buff, 3, "%lu", (size_t)json_integer_value(num));
    concatenate(&button, num_buff);
    concatenate(&button, "\" \"");
    concatenate(&button, (char*)json_string_value(name));
    concatenate(&button, "\")");
    concatenate(&output, button);
    free(button);
  }
  concatenate(&output, ")");
  free(num_buff);
  pclose(pipe);
  json_decref(ws_info);
  /* printf("%s\n", output); */
  eww_update_variable("workspace", output);
  free(output);
}

int main() {
  get_ws_info(NULL);
  ipc_struct* ipc = calloc(1, sizeof(ipc_struct));
  ipc_start(ipc);
  char m[] = "[\"workspace\"]";
  sock_subscribe(ipc, m, strlen(m));
  while (1) {
    sock_handle_event(ipc, &get_ws_info);
  }
  ipc_end(ipc);
  return 0;
}
