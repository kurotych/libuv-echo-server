#ifndef ECHO_SERVER_TCP_ECHO_H
#define ECHO_SERVER_TCP_ECHO_H
#include <uv.h>

// allocates an heap an return uv_tcp_t object
uv_tcp_t * init_echo_tcp_server(uv_loop_t *loop, const char* address, uint16_t port);

#endif //ECHO_SERVER_TCP_ECHO_H
