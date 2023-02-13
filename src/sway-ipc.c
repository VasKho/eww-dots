#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sway-ipc.h"

#define STR_BUFF_SIZE 256

char* get_socket_path() {
  char* env = getenv("SWAYSOCK");
  if (env != NULL) {
    return env;
  }
  char* str = calloc(STR_BUFF_SIZE, sizeof(char));
  size_t str_size = STR_BUFF_SIZE;
  size_t ind = 0;
  FILE* in;
  char tmp;
  if ((in = popen("sway --get-socketpath 2 > /dev/null", "r")) == NULL) {
    return NULL;
  }
  while ((tmp = fgetc(in)) != EOF) {
    str[ind++] = tmp;
    if (ind == str_size) {
      str_size += STR_BUFF_SIZE;
      str = realloc(str, str_size);
    }
  }
  pclose(in);
  if (ind == 0) {
    return NULL;
  }
  if (str[ind-1] == '\n') {
    str[ind-1] = '\0';
  }
  return str;
}

int sock_open(char* socket_path) {
  int32_t fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    return -1;
  }
  fcntl(fd, F_SETFD, FD_CLOEXEC);
  int len = sizeof(struct sockaddr_un);
  struct sockaddr_un* addr = calloc(1, len);
  memset(addr, 0, len);
  addr->sun_family = AF_UNIX;
  strncpy(addr->sun_path, socket_path, sizeof(addr->sun_path) - 1);
  addr->sun_path[sizeof(addr->sun_path) - 1] = 0;
  if (connect(fd, (struct sockaddr*)addr, len) == -1) {
    free(addr);
    return -1;
  }
  free(addr);
  return fd;
}

void ipc_start(ipc_struct* ipc) {
  if (ipc == NULL) {
    return;
  }
  char* socket_path = get_socket_path();
  ipc->fd = sock_open(socket_path);
  ipc->fd_event = sock_open(socket_path);
}

void ipc_end(ipc_struct* ipc) {
  if (ipc == NULL) {
    return;
  }
  if (ipc->fd > 0) {
    write(ipc->fd, "close-sway-ipc", 14);
    close(ipc->fd);
  }
  if (ipc->fd_event > 0) {
    write(ipc->fd_event, "close-sway-ipc", 14);
    close(ipc->fd_event);
  }
  free(ipc);
  ipc = NULL;
}

ipc_response sock_recv(ipc_struct* ipc, int fd) {
  char header[IPC_HEADER_SIZE];
  uint32_t* data32 = (uint32_t*)(header + IPC_MAGIC_SIZE);
  size_t total = 0;
  while (total < IPC_HEADER_SIZE) {
    int res = recv(fd, header+total, IPC_HEADER_SIZE-total, 0);
    if (ipc->fd_event == -1 || ipc->fd == -1) {
      /* IPC is closed so just return an empty response */
      return NULL_RESPONCE;
    }
    if (res <= 0) {
      return NULL_RESPONCE;
    }
    total += res;
  }
  char magic[IPC_MAGIC_SIZE];
  memcpy(magic, header, IPC_MAGIC_SIZE);
  if (strncmp(magic, IPC_MAGIC, IPC_MAGIC_SIZE) != 0) {
    return NULL_RESPONCE;
  }
  total = 0;
  char* payload = calloc(data32[0]+1, sizeof(char));
  while (total < data32[0]) {
    int res = recv(fd, payload+total, data32[0]-total, 0);
    if (res < 0) {
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }
      return NULL_RESPONCE;
    }
    total += res;
  }
  return (ipc_response){ data32[0], data32[1], payload };
}

ipc_response sock_send(ipc_struct* ipc, int fd, uint32_t type, char* payload, size_t payload_size) {
  char header[IPC_HEADER_SIZE];
  uint32_t* data32 = (uint32_t*)(header + IPC_MAGIC_SIZE);
  memcpy(header, IPC_MAGIC, IPC_MAGIC_SIZE);
  data32[0] = payload_size;
  data32[1] = type;
  if (send(fd, header, IPC_HEADER_SIZE, 0) == -1) {
    return NULL_RESPONCE;
  }
  if (send(fd, payload, payload_size, 0) == -1) {
    return NULL_RESPONCE;
  }
  return sock_recv(ipc, fd);
}

void sock_subscribe(ipc_struct* ipc, char* payload, size_t payload_size) {
  ipc_response response = sock_send(ipc, ipc->fd_event, 2, payload, payload_size);
  if (strcmp(response.payload, "{\"success\": true}") != 0) {
    return;
  }
}

void sock_handle_event(ipc_struct* ipc, void (*func)(ipc_response*)) {
  ipc_response response = sock_recv(ipc, ipc->fd_event);
  func(&response);
  free(response.payload);
}
