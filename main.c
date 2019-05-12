#include <stdlib.h>
#include "udp_echo.h"
#include "tcp_echo.h"

#define UDP_SERVER_PORT 8888
#define TCP_SERVER_PORT 9999

static uv_udp_t* udp_socket = NULL;
static uv_tcp_t* tcp_socket = NULL;

static void on_walk_cleanup(uv_handle_t* handle, void* data)
{
    (void)data;
    uv_close(handle, NULL);
}

static void on_server_exit()
{
    printf("on_server_exit\n");
    // clean all stuff
    uv_stop(uv_default_loop());
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_walk(uv_default_loop(), on_walk_cleanup, NULL);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_loop_close(uv_default_loop());
    free(udp_socket);
    free(tcp_socket);
    exit(0);
}

static void on_signal(uv_signal_t* signal, int signum)
{
    (void)signum;
    printf("on_signal\n");

    uv_signal_stop(signal);
    if(tcp_socket)
        uv_close((uv_handle_t *) tcp_socket, on_server_exit);
    else if(udp_socket)
        uv_close((uv_handle_t *) udp_socket, on_server_exit);
    else
        on_server_exit();
}

static inline void init_signal(uv_loop_t *loop, uv_signal_t* signal, int signum)
{
    uv_signal_init(loop, signal);
    uv_signal_start(signal, on_signal, signum);
}

int main(int argc, const char** argv)
{
    (void)argc; (void) argv;
    uv_loop_t *loop = uv_default_loop();

    uv_signal_t sigkill, sigterm, sigint;
    init_signal(loop, &sigkill, SIGKILL);
    init_signal(loop, &sigterm, SIGTERM);
    init_signal(loop, &sigint, SIGINT);

    udp_socket = init_echo_udp_server(loop, "0.0.0.0", UDP_SERVER_PORT);
    tcp_socket = init_echo_tcp_server(loop, "0.0.0.0", TCP_SERVER_PORT);

    printf("Server listening on: UDP port: %d, TCP port: %d\n", UDP_SERVER_PORT, TCP_SERVER_PORT);
    fflush(stdout);
    return uv_run(loop, UV_RUN_DEFAULT);
}