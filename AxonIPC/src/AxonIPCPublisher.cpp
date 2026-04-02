#include "AxonIPCPublisher.h"

namespace AxonIPC
{
  AxonIPCPublisher::AxonIPCPublisher(Context& context, const Path& subscriberPath, const Path& publisherPath)
    : m_publisher(context, subscriberPath, publisherPath)
  {}

  AxonIPCPublisher::AxonIPCPublisher(AxonIPCPublisher&& other) noexcept
    : m_publisher(std::move(other.m_publisher))
  {}

  AxonIPCPublisher& AxonIPCPublisher::operator=(AxonIPCPublisher&& other) noexcept
  {
    AxonIPCPublisher(std::move(other)).Swap(*this);
    return *this;
  }

  void AxonIPCPublisher::Swap(AxonIPCPublisher& other)
  {
    std::swap(m_publisher, other.m_publisher);
  }

  void AxonIPCPublisher::Publish(const int type, const std::string_view& payload)
  {
    m_publisher.Publish(type, payload);
  }
}
