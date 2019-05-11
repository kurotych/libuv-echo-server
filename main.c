#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

uv_loop_t *loop;
uv_udp_t recv_socket;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

/// called after the data was sent
static void on_send(uv_udp_send_t* req, int status)
{
    free(req);
    if (status) {
        fprintf(stderr, "uv_udp_send_cb error: %s\n", uv_strerror(status));
    }
}

void on_read(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags) {

    if (nread < 0) {
        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) req, NULL);
        free(buf->base);
        return;
    }

    if (nread > 0) {
        char sender[17] = { 0 };
        uv_ip4_name((const struct sockaddr_in*) addr, sender, 16);
        fprintf(stderr, "Recv from %s\n", sender);

        uv_udp_send_t* res = malloc(sizeof(uv_udp_send_t));
        uv_buf_t buff = uv_buf_init(buf->base, nread);
        uv_udp_send(res, req, &buff, 1, addr, on_send);
    }

    free(buf->base);
  //  uv_udp_recv_stop(req);
}

static void on_walk_cleanup(uv_handle_t* handle, void* data)
{
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
    exit(0);
}

static void on_signal(uv_signal_t* signal, int signum)
{
    printf("on_signal\n");
    if(uv_is_active((uv_handle_t*)&recv_socket)) {
        uv_udp_recv_stop(&recv_socket);
    }
    uv_close((uv_handle_t*) &recv_socket, on_server_exit);
    uv_signal_stop(signal);
}

int main() {
    loop = uv_default_loop();
    uv_signal_t sigkill;
    uv_signal_init(loop, &sigkill);
    uv_signal_start(&sigkill, on_signal, SIGKILL);

    uv_signal_t sigterm;
    uv_signal_init(loop, &sigterm);
    uv_signal_start(&sigkill, on_signal, SIGTERM);

    uv_signal_t sigint;
    uv_signal_init(loop, &sigint);
    uv_signal_start(&sigkill, on_signal, SIGINT);


    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", 8888, &recv_addr);

    uv_udp_init(loop, &recv_socket);

    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);

    return uv_run(loop, UV_RUN_DEFAULT);
}