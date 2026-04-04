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

## How to use
Two programs, North and South, both residing in the same directory so each one can find the unix socket path of the opposing program.
```cpp
// Program called North
#include "AxonIPC/Context.h"
#include "AxonIPC/AxonIPCSubscriber.h"
#include "AxonIPC/AxonIPCPublisher.h"
#include <string>
#include <iostream>
int main()
{
    AxonIPC::PlatformContext context;
    AxonIPC::AxonIPCPublisher publisher(context, AxonIPC::Path("./north"));
    AxonIPC::AxonIPCSubscriber subscriber(context, AxonIPC::Path("./north"));
    subscriber.GetDispatcher()->RegisterSubscriber(42, [&](const int type, const std::string_view& publisherPath, const std::string_view& payload)
    {
      std::cout << publisherPath << ": " << payload << std::endl;
    });

    std::cout << "Enter \"exit\" to exit." << std::endl;
    while(true)
    {
        std::cout << "Payload: ";
        std::string payload;
        std::cin >> payload;
        if (payload == "exit")
            break;

        publisher.Publish(42, payload, AxonIPC::Path("./south"));
    }
}
```

```cpp
// Program called South
#include "AxonIPC/Context.h"
#include "AxonIPC/AxonIPCSubscriber.h"
#include "AxonIPC/AxonIPCPublisher.h"
#include <string>
#include <iostream>
int main()
{
    AxonIPC::PlatformContext context;
    AxonIPC::AxonIPCPublisher publisher(context, AxonIPC::Path("./south"));
    AxonIPC::AxonIPCSubscriber subscriber(context, AxonIPC::Path("./south"));
    subscriber.GetDispatcher()->RegisterSubscriber(42, [&](const int type, const std::string_view& publisherPath, const std::string_view& payload)
    {
      std::cout << publisherPath << ": " << payload << std::endl;
    });

    std::cout << "Enter \"exit\" to exit." << std::endl;
    while(true)
    {
        std::cout << "Payload: ";
        std::string payload;
        std::cin >> payload;
        if (payload == "exit")
            break;

        publisher.Publish(42, payload, AxonIPC::Path("./north"));
    }
}
```
> [!NOTE]
> When using AF_UNIX with SOCK_DGRAM on Linux, we are guaranteed to receive a full packet in a single read call, provided the buffer is large enough. Unlike standard UDP over a network, Unix domain datagram sockets on Linux are always reliable and do not reorder or drop packets under normal conditions.
