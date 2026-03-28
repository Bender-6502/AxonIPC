#pragma once
#include "Context.h"

namespace AxonIPC
{
  class Socket
  {
  public:
    Socket() = default;
    explicit Socket(Context& context);
    Socket(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&& other) noexcept;
   ~Socket();

    void Swap(Socket& other);
    void CloseSocket();

    Context* ContextHandle() const;
    socket_t SocketHandle() const;

    operator bool() const;

  protected:
    Context* m_context = nullptr;
    socket_t m_sock{};
  };
}
