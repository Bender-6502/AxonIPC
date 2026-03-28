#pragma once
#include <map>
#include <functional>
#include <string_view>
#include <mutex>

namespace AxonIPC
{
    class Dispatcher
    {
    public:
        Dispatcher() = default;
        Dispatcher(const Dispatcher& other) = delete;
        Dispatcher(Dispatcher&& other) noexcept;
        Dispatcher& operator=(const Dispatcher& other) = delete;
        Dispatcher& operator=(Dispatcher&& other) noexcept;
       ~Dispatcher() = default;

        void Swap(Dispatcher& other);

        void Dispatch(int type, const std::string_view& payload);

        void DeRegisterSubscriber(int type);

        template <typename Obj>
        void RegisterSubscriber(int type, void(Obj::*method)(int, const std::string_view&), Obj* obj)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_subscribers.insert(std::make_pair(type, std::bind(method, obj)));
        }

        template <typename Func>
        void RegisterSubscriber(int type, Func&& func)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_subscribers.insert(std::make_pair(type, func));
        }

    protected:
        std::mutex m_mutex{};
        std::map<int, std::function<void(int, const std::string_view&)> > m_subscribers{};
    };
}
