<img src="https://github.com/Bender-6502/AxonIPC/blob/master/AxonIPC/assets/AxonIPC.png" width="25%"></img>
# AxonIPC

#### IPC Message library for Linux, tested on Ubuntu C++20/CMake

## Design goals
- Local machine IPC
- Simple pub/sub using point-2-point unix paths
- Reusable class interface
- No third-party libraries
- Clear separation between Publisher and Subscriber

## Design Overview
- Linux Transport: UNIX domain sockets (AF_UNIX, SOCK_DGRAM)
- Pattern: Publisher broadcasts messages, Subscribers bind to their own socket path
- Message format: Binary

## Testing
- Google tests and mocks

> [!NOTE]
> When using AF_UNIX with SOCK_DGRAM on Linux, we are guaranteed to receive a full packet in a single read call, provided the buffer is large enough. Unlike standard UDP over a network, Unix domain datagram sockets on Linux are always reliable and do not reorder or drop packets under normal conditions.
