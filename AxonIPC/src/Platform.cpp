#include "Platform.h"
#include "Path.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef __linux__
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#endif

namespace AxonIPC
{
  int LastErrorCode()
  {
    return errno;
  }

  socket_t CreateSocket()
  {
#ifdef __linux__
    return ::socket(AF_UNIX, SOCK_DGRAM, 0);
#else
    return 0;
#endif
  }

  void SocketClose(socket_t s)
  {
#ifdef __linux__
    if (s)
      ::close(s);
#endif
  }

  void Shutdown(socket_t s)
  {
#ifdef __linux__
    if (s)
      ::shutdown(s, SHUT_RDWR);
#endif
  }

  void* CreatePathToAddr(const std::filesystem::path& path)
  {
#ifdef __linux__
    auto* addr = new sockaddr_un;
    std::memset(addr, 0, sizeof(sockaddr_un));
    addr->sun_family = AF_UNIX;
    path.string().copy(addr->sun_path, sizeof(addr->sun_path) - 1);

    return addr;
#else
    return nullptr;
#endif
  }

  void FreePathToAddr(void* ptr)
  {
#ifdef __linux__
    delete static_cast<sockaddr_un*>(ptr);
#endif
  }

  int PathToAddrSize()
  {
#ifdef __linux__
    return sizeof(sockaddr_un);
#else
    return 0;
#endif
  }

  bool Bind(socket_t s, const Path& path)
  {
#ifdef __linux__
    return ::bind(s, static_cast<const sockaddr*>(path.NativeHandle()), path.Size()) == 0;
#else
    return true;
#endif
  }

  void Unbind(const Path& path)
  {
#ifdef __linux__
    ::unlink(path.NativePath().string().c_str());
#endif
  }

  bool SendTo(socket_t s, const void* data, const size_t size, const Path& path)
  {
#ifdef __linux__
    return ::sendto(s, data, static_cast<int>(size), 0, static_cast<const sockaddr*>(path.NativeHandle()), path.Size()) != -1;
#else
    return true;
#endif
  }

  bool RecvFrom(socket_t s, void* data, const size_t size, size_t& bytesRead)
  {
#ifdef __linux__
    if (const auto read = ::recv(s, data, static_cast<int>(size), 0); read != -1)
    {
      bytesRead = static_cast<size_t>(read);
      return true;
    }
    return false;
#else
    return true;
#endif
  }
}
