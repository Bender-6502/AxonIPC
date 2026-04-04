#pragma once
#include "Socket.h"
#include "Path.h"
#include <string_view>
#include <array>
#include <mutex>

namespace AxonIPC
{
  constexpr auto PublisherBufferSize = 10000;

  class Publisher
  {
  public:
    Publisher() = default;
    Publisher(Context& context, const Path& subscriberPath);
    Publisher(const Publisher&) = delete;
    Publisher(Publisher&& other) noexcept;
    Publisher& operator=(const Publisher&) = delete;
    Publisher& operator=(Publisher&& other) noexcept;
   ~Publisher() = default;

    void Swap(Publisher& other);

    void Publish(int type, const std::string_view& payload, const Path& publisherPath);

  private:
    Socket m_socket{};
    Path m_subscriberPath{};
    std::array<char, PublisherBufferSize> m_buf{};
    std::mutex m_mutex;
  };
}
