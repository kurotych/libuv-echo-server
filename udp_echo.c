#include "udp_echo.h"
#include <stdlib.h>

uv_udp_t recv_socket;

/// called after the data was sent
static void on_send(uv_udp_send_t* req, int status)
{
    free(req);
    if (status) {
        fprintf(stderr, "uv_udp_send_cb error: %s\n", uv_strerror(status));
    }
}

void on_read(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf,
             const struct sockaddr *addr, unsigned flags)
{
    (void)flags;
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
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    (void)handle;
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

uv_udp_t* init_echo_udp_server(uv_loop_t *loop, const char* address, uint16_t port)
{
    struct sockaddr_in recv_addr;
    uv_ip4_addr(address, port, &recv_addr);

    uv_udp_init(loop, &recv_socket);

    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);
    return &recv_socket;
}