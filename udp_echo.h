#ifndef ECHO_SERVER_UDP_ECHO_SERVER_H
#define ECHO_SERVER_UDP_ECHO_SERVER_H

#include <uv.h>

uv_udp_t* init_echo_udp_server(uv_loop_t *loop, const char* address, uint16_t port);

#endif //ECHO_SERVER_UDP_ECHO_SERVER_H
