#include "AxonIPCSubscriber.h"
#include <span>

namespace AxonIPC
{
  AxonIPCSubscriber::AxonIPCSubscriber(Context& context, const Path& subscriberPath)
    : m_subscriber(context, subscriberPath)
  {
    m_thread = std::thread([&]
      {
        try
        {
          while (!m_abort)
          {
            int type{};
            std::string_view publisher, payload;
            m_subscriber.Receive(type, publisher, payload);
            m_disp.Dispatch(type, publisher, payload);
            std::this_thread::yield();
          }
        }
        catch (...) {}
        m_abort = true;
      });
  }

  AxonIPCSubscriber::AxonIPCSubscriber(AxonIPCSubscriber&& other) noexcept
    : m_subscriber(std::move(other.m_subscriber))
    , m_abort(other.m_abort)
    , m_thread(std::move(other.m_thread))
  {}

  AxonIPCSubscriber& AxonIPCSubscriber::operator=(AxonIPCSubscriber&& other) noexcept
  {
    AxonIPCSubscriber(std::move(other)).Swap(*this);
    return *this;
  }

  AxonIPCSubscriber::~AxonIPCSubscriber()
  {
    try
    {
      m_abort = true;
      if (m_thread.joinable()) {
        m_subscriber.ShutDown();
        m_thread.join();
      }
    }
    catch (...) {}
  }

  void AxonIPCSubscriber::Swap(AxonIPCSubscriber& other)
  {
    std::swap(m_subscriber, other.m_subscriber);
    std::swap(m_abort, other.m_abort);
    std::swap(m_thread, other.m_thread);
  }

  Dispatcher* AxonIPCSubscriber::GetDispatcher()
  {
    return &m_disp;
  }
}
