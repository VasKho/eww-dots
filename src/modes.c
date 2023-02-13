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

void get_mode_info(ipc_response* resp) {
  if (resp == NULL) {
    eww_update_variable("rev-mod", "false");
    eww_update_variable("rev-mus", "true");
    return;
  }
  json_t* ws_info = json_loadb(resp->payload, resp->size,  0, NULL);
  json_t* value = json_object_get(ws_info, "change");
  char* name = (char*)json_string_value(value);
  printf("%s\n", name);
  if (strncmp(name, "default", strlen("default")) != 0) {
    eww_update_variable("modes", name);
    eww_update_variable("rev-mod", "true");
    eww_update_variable("rev-mus", "false");
  } else {
    eww_update_variable("rev-mod", "false");
    eww_update_variable("rev-mus", "true");
  }
  json_decref(ws_info);
}

int main() {
  get_mode_info(NULL);
  ipc_struct* ipc = calloc(1, sizeof(ipc_struct));
  ipc_start(ipc);
  char m[] = "[\"mode\"]";
  sock_subscribe(ipc, m, strlen(m));
  while (1) {
    sock_handle_event(ipc, &get_mode_info);
  }
  ipc_end(ipc);
  return 0;
}
