#include "Subscriber.h"
#include "BinaryReader.h"
#include "Socket.h"
#include "Path.h"
#include <string>
#include <stdexcept>
#include <format>

namespace AxonIPC
{
  Subscriber::Subscriber(Context& context, const Path& path)
    : m_socket(context)
    , m_path(path)
  {
    OpenUp();
  }

  Subscriber::Subscriber(Subscriber&& other) noexcept
    : m_socket(std::move(other.m_socket))
    , m_path(std::move(other.m_path))
  {}

  Subscriber& Subscriber::operator=(Subscriber&& other) noexcept
  {
    Subscriber(std::move(other)).Swap(*this);
    return *this;
  }

  Subscriber::~Subscriber()
  {
    ShutDown();
  }

  void Subscriber::Swap(Subscriber& other)
  {
    std::swap(m_socket, other.m_socket);
    std::swap(m_path, other.m_path);
  }

  void Subscriber::OpenUp()
  {
    if (!m_socket.ContextHandle()->Bind(m_socket.SocketHandle(), m_path))
      throw std::runtime_error(std::format("Failed to subscribe: {}", m_socket.ContextHandle()->LastErrorCode()));
  }

  void Subscriber::ShutDown()
  {
    if (m_socket) {
      m_socket.ContextHandle()->Unbind(m_path);
      m_socket.CloseSocket();
    }
  }

  void Subscriber::Receive(int& type, std::string_view& payload)
  {
    size_t bytesRead = 0;

    // Read the packet
    if (!m_socket.ContextHandle()->RecvFrom(m_socket.SocketHandle(), m_buf.data(), m_buf.size(), bytesRead))
      throw std::runtime_error(std::format("Failed to recv: {}", m_socket.ContextHandle()->LastErrorCode()));

    // Extract the members from the packet
    BinaryReader reader(std::span<char>(m_buf.data(), bytesRead));
    reader.Read(type);
    reader.Read(payload);
  }
}
