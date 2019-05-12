# libuv-echo-server

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
