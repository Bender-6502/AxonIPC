#include "Context.h"
#include "Platform.h"

namespace AxonIPC
{
  int PlatformContext::LastErrorCode() const
  {
    return AxonIPC::LastErrorCode();
  }

  socket_t PlatformContext::CreateSocket()
  {
    return AxonIPC::CreateSocket();
  }

  void PlatformContext::SocketClose(socket_t s)
  {
    AxonIPC::SocketClose(s);
  }

  void PlatformContext::Shutdown(socket_t s)
  {
    AxonIPC::Shutdown(s);
  }

  bool PlatformContext::Bind(socket_t s, const Path& path)
  {
    return AxonIPC::Bind(s, path);
  }

  void PlatformContext::Unbind(const Path& path)
  {
    AxonIPC::Unbind(path);
  }

  bool PlatformContext::SendTo(socket_t s, const void* data, const size_t size, const Path& path)
  {
    return AxonIPC::SendTo(s, data, size, path);
  }

  bool PlatformContext::RecvFrom(socket_t s, void* data, const size_t size, size_t& bytesRead)
  {
    return AxonIPC::RecvFrom(s, data, size, bytesRead);
  }
}
