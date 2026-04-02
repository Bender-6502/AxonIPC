#pragma once
#include "Subscriber.h"
#include "Dispatcher.h"
#include <thread>

namespace AxonIPC
{
  class AxonIPCSubscriber
  {
  public:
    AxonIPCSubscriber() = default;
    AxonIPCSubscriber(const AxonIPCSubscriber&) = delete;
    AxonIPCSubscriber(Context& context, const Path& subscriberPath);
    AxonIPCSubscriber(AxonIPCSubscriber&& other) noexcept;
    AxonIPCSubscriber& operator=(const AxonIPCSubscriber&) = delete;
    AxonIPCSubscriber& operator=(AxonIPCSubscriber&& other) noexcept;
   ~AxonIPCSubscriber();

    void Swap(AxonIPCSubscriber& other);

    Dispatcher* GetDispatcher();

  private:
    Subscriber m_subscriber;
    Dispatcher m_disp;
    bool m_abort = false;
    std::thread m_thread{};
  };
}