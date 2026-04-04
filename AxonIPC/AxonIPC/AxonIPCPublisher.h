#pragma once
#include "Publisher.h"
#include <string_view>

namespace AxonIPC
{
  class AxonIPCPublisher
  {
  public:
    AxonIPCPublisher() = default;
    AxonIPCPublisher(Context& context, const Path& subscriberPath);
    AxonIPCPublisher(const AxonIPCPublisher&) = delete;
    AxonIPCPublisher(AxonIPCPublisher&& other) noexcept;
    AxonIPCPublisher& operator=(const AxonIPCPublisher&) = delete;
    AxonIPCPublisher& operator=(AxonIPCPublisher&& other) noexcept;
   ~AxonIPCPublisher() = default;

    void Swap(AxonIPCPublisher& other);

    void Publish(int type, const std::string_view& payload, const Path& publisherPath);

  private:
    Publisher m_publisher;
  };
}