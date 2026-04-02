// Program called South
#include "AxonIPC/Context.h"
#include "AxonIPC/AxonIPCSubscriber.h"
#include "AxonIPC/AxonIPCPublisher.h"
#include <string>
#include <iostream>
int main()
{
    AxonIPC::PlatformContext subscriberContext;
    AxonIPC::AxonIPCSubscriber subscriber(subscriberContext, AxonIPC::Path("./south"));
    subscriber.GetDispatcher()->RegisterSubscriber(42, [&](const int type, const std::string_view& publisher, const std::string_view& payload)
    {
      std::cout << publisher << ": " << payload << std::endl;
    });

    AxonIPC::PlatformContext publisherContext;
    AxonIPC::AxonIPCPublisher publisher(publisherContext, AxonIPC::Path("./south"), AxonIPC::Path("./north"));
    std::cout << "Enter \"exit\" to exit." << std::endl;
    while(true)
    {
        std::cout << "Payload: ";
        std::string payload;
        std::cin >> payload;
        if (payload == "exit")
            break;

        publisher.Publish(42, payload);
    }
}
