#include "Socket.h"
#include <stdexcept>
#include <format>

namespace AxonIPC
{
  Socket::Socket(Context& context)
    : m_context(&context)
  {
    m_sock = m_context->CreateSocket();
    if (m_sock < 0)
      throw std::runtime_error(std::format("Failed to open message queue socket: {}", m_context->LastErrorCode()));
  }

  Socket::Socket(Socket&& other) noexcept
    : m_context(other.m_context)
    , m_sock(other.m_sock)
  {
    other.m_context = nullptr;
    other.m_sock = 0;
  }

  Socket& Socket::operator=(Socket&& other) noexcept
  {
    Socket(std::move(other)).Swap(*this);
    return *this;
  }

  Socket::~Socket()
  {
    CloseSocket();
  }

  void Socket::Swap(Socket& other)
  {
    std::swap(m_context, other.m_context);
    std::swap(m_sock, other.m_sock);
  }

  void Socket::CloseSocket()
  {
    if (m_sock) {
      m_context->Shutdown(m_sock);
      m_context->SocketClose(m_sock);
    }
    m_sock = 0;
  }

  Context* Socket::ContextHandle() const
  {
    return m_context;
  }

  socket_t Socket::SocketHandle() const
  {
    return m_sock;
  }

  Socket::operator bool() const
  {
    return m_sock != 0;
  }
}
