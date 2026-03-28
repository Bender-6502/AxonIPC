#include "AxonIPCSubscriber.h"
#include <span>

namespace AxonIPC
{
    AxonIPCSubscriber::AxonIPCSubscriber(Context &context, const Path &path)
        : m_subscriber(context, path)
    {
        m_thread = std::thread([&]
        {
            try
            {
                while (!m_abort)
                {
                    int type{};
                    std::string_view payload;
                    m_subscriber.Receive(type, payload);
                    m_disp.Dispatch(type, payload);
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
        m_abort = true;
        if (m_thread.joinable()) {
            m_subscriber.ShutDown();
            m_thread.join();
        }
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
