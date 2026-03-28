#pragma once
#include <filesystem>
#include <string>

namespace AxonIPC
{
  class Path;
  using socket_t = int;

  int LastErrorCode();

  socket_t CreateSocket();
  void SocketClose(socket_t s);

  void Shutdown(socket_t s);

  void* CreatePathToAddr(const std::filesystem::path& path);
  void FreePathToAddr(void* ptr);
  int PathToAddrSize();

  bool Bind(socket_t s, const Path& path);
  void Unbind(const Path& path);

  bool SendTo(socket_t s, const void* data, size_t size, const Path& path);
  bool RecvFrom(socket_t s, void* data, size_t size, size_t& bytesRead);
}
