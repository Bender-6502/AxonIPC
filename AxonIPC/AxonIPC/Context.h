#pragma once
#include "Platform.h"

namespace AxonIPC
{
  class Context
  {
  public:
    virtual ~Context() = default;

    virtual int LastErrorCode() const = 0;

    virtual socket_t CreateSocket() = 0;
    virtual void SocketClose(socket_t s) = 0;

    virtual void Shutdown(socket_t s) = 0;

    virtual bool Bind(socket_t s, const Path& path) = 0;
    virtual void Unbind(const Path& path) = 0;

    virtual bool SendTo(socket_t s, const void* data, size_t size, const Path& path) = 0;
    virtual bool RecvFrom(socket_t s, void* data, size_t size, size_t& bytesRead) = 0;
  };

  class PlatformContext : public Context
  {
  public:
    PlatformContext() = default;
   ~PlatformContext() override = default;

    int LastErrorCode() const override;

    socket_t CreateSocket() override;
    void SocketClose(socket_t s) override;

    void Shutdown(socket_t s) override;

    bool Bind(socket_t s, const Path& path) override;
    void Unbind(const Path& path) override;

    bool SendTo(socket_t s, const void* data, size_t size, const Path& path) override;
    bool RecvFrom(socket_t s, void* data, size_t size, size_t& bytesRead) override;
  };
}
