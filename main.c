#include <stdlib.h>
#include "udp_echo.h"

#define SERVER_PORT 8888

static uv_udp_t* udp_socket = NULL;

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
    exit(0);
}

static void on_signal(uv_signal_t* signal, int signum)
{
    (void)signum;
    printf("on_signal\n");
    if(uv_is_active((uv_handle_t*)udp_socket)) {
        uv_udp_recv_stop(udp_socket);
    }
    uv_close((uv_handle_t*) udp_socket, on_server_exit);
    uv_signal_stop(signal);
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

    udp_socket = init_echo_udp_server(loop, "0.0.0.0", SERVER_PORT);

    printf("Server listening on: %d\n", SERVER_PORT);
    fflush(stdout);
    return uv_run(loop, UV_RUN_DEFAULT);
}