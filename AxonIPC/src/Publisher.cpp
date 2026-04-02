#include "Publisher.h"
#include "BinaryWriter.h"
#include <stdexcept>
#include <format>

namespace AxonIPC
{
  Publisher::Publisher(Context& context, const Path& subscriberPath, const Path& publisherPath)
    : m_socket(context)
    , m_subscriberPath(subscriberPath)
    , m_publisherPath(publisherPath)
  {}

  Publisher::Publisher(Publisher&& other) noexcept
    : m_socket(std::move(other.m_socket))
    , m_subscriberPath(std::move(other.m_subscriberPath))
    , m_publisherPath(std::move(other.m_publisherPath))
  {}

  Publisher& Publisher::operator=(Publisher&& other) noexcept
  {
    Publisher(std::move(other)).Swap(*this);
    return *this;
  }

  void Publisher::Swap(Publisher& other)
  {
    std::swap(m_socket, other.m_socket);
    std::swap(m_subscriberPath, other.m_subscriberPath);
    std::swap(m_publisherPath, other.m_publisherPath);
  }

  void Publisher::Publish(const int type, const std::string_view& payload)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    BinaryWriter writer(m_buf);
    writer.Write(type);
    writer.Write(m_subscriberPath);
    writer.Write(payload);

    if (!m_socket.ContextHandle()->SendTo(m_socket.SocketHandle(), m_buf.data(), writer.Size(), m_publisherPath))
      throw std::runtime_error(std::format("Failed to publish: {}", m_socket.ContextHandle()->LastErrorCode()));
  }
}
