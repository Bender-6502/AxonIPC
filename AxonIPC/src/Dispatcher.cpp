#include "Dispatcher.h"

namespace AxonIPC
{
  Dispatcher::Dispatcher(Dispatcher&& other) noexcept
    : m_subscribers(std::move(other.m_subscribers))
  {}

  Dispatcher& Dispatcher::operator=(Dispatcher&& other) noexcept
  {
    Dispatcher(std::move(other)).Swap(*this);
    return *this;
  }

  void Dispatcher::Swap(Dispatcher& other)
  {
    std::swap(m_subscribers, other.m_subscribers);
  }

  void Dispatcher::Dispatch(const int type, const std::string_view& payload)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (const auto sub = m_subscribers.find(type); sub != m_subscribers.end())
      sub->second(type, payload);
  }

  void Dispatcher::DeRegisterSubscriber(const int type)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (const auto sub = m_subscribers.find(type); sub != m_subscribers.end())
      m_subscribers.erase(sub);
  }
}
