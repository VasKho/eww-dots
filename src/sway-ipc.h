#ifndef _SWAY_IPC_H_
#define _SWAY_IPC_H_

#include <stdio.h>
#include <stdint.h>

#define IPC_MAGIC "i3-ipc"
#define IPC_MAGIC_SIZE ( strlen(IPC_MAGIC) )
#define IPC_HEADER_SIZE ( IPC_MAGIC_SIZE+8 )
#define NULL_RESPONCE ( (ipc_response){ 0, 0, "" } )

typedef struct {
  uint32_t size;
  uint32_t type;
  char* payload;
} ipc_response;

typedef struct {
  int fd;
  int fd_event;
} ipc_struct;

char* get_socket_path();

void ipc_start(ipc_struct* ipc);
void ipc_end(ipc_struct* ipc);

ipc_response sock_recv(ipc_struct* ipc, int fd);
ipc_response sock_send(ipc_struct* ipc, int fd, uint32_t type, char* payload, size_t payload_size);
void sock_subscribe(ipc_struct* ipc, char* payload, size_t payload_size);
void sock_handle_event(ipc_struct* ipc, void (*func)(ipc_response*));

#endif
