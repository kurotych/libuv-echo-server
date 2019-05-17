# libuv-echo-server
[![Build Status](https://travis-ci.org/armatusmiles/libuv-echo-server.svg?branch=master)](https://travis-ci.org/armatusmiles/libuv-echo-server)

Tested with valgrind and flood udp script. Memory leaks are not detected

## Build & Run 
```
mkdir build && cd build
cmake ../
make
./echo_server
```

By default listening ports:
- TCP 8888
- UDP 9999

> libuv version: 1.22.0
