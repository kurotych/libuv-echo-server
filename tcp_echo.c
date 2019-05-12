#include "tcp_echo.h"
#include <stdlib.h>

static uv_loop_t* _loop;

static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    (void)handle;
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

static void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

static void free_handle(uv_handle_t* handle)
{
    free(handle);
}

static void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        }
        uv_close((uv_handle_t*) client, free_handle);
    } else if (nread > 0) {
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base) {
        free(buf->base);
    }
}

static void on_new_connection(uv_stream_t *server, int status)
{
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(_loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
    } else {
        uv_close((uv_handle_t*) client, NULL);
    }
}


uv_tcp_t * init_echo_tcp_server(uv_loop_t *loop, const char* address, uint16_t port)
{
    // for remove this global loop need change module to object design
    // I will not doing it in "example project"
    _loop = loop;
    uv_tcp_t *server = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, server);

    struct sockaddr_in sock_addr;
    uv_ip4_addr(address, port, &sock_addr);

    uv_tcp_bind(server, (const struct sockaddr*)&sock_addr, 0);
    int r = uv_listen((uv_stream_t*)server, 128, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return NULL;
    }
    return server;
}