#include <uv.h>

/// backlog queue – the maximum length of queued connections
/// for tcp connection
#define DEFAULT_BACKLOG 10000

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    (void)handle;
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_new_connection(uv_loop_t *loop, uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, on_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

void init_tcp_server(uv_loop_t *loop, uv_tcp_t server, const char* address, uint16_t port)
{
    struct sockaddr_in recv_addr;

    uv_tcp_init(loop, &server);

    uv_ip4_addr(address, port, &recv_addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&recv_addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    }
}