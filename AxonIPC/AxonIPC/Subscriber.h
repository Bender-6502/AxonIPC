#pragma once
#include "Socket.h"
#include "Path.h"
#include <string_view>
#include <span>

namespace AxonIPC
{
  constexpr auto SubscriberBufferSize = 10000;

  class Subscriber
  {
  public:

    Subscriber() = default;
    Subscriber(Context& context, const Path& path);
    Subscriber(const Subscriber&) = delete;
    Subscriber(Subscriber&& other) noexcept;
    Subscriber& operator=(const Subscriber&) = delete;
    Subscriber& operator=(Subscriber&& other) noexcept;
   ~Subscriber();

    void Swap(Subscriber& other);

    void OpenUp();
    void ShutDown();
    void Receive(int& type, std::string_view& payload);

  private:
    Socket m_socket{};
    Path m_path{};
    std::array<char, SubscriberBufferSize> m_buf{};
  };
}
