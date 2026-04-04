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
