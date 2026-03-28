#pragma once
#include <filesystem>

namespace AxonIPC
{
  class Path
  {
  public:
    Path() = default;
    Path(const std::filesystem::path& path);
    Path(const Path& other) noexcept;
    Path(Path&& other) noexcept;
    Path& operator=(const Path& other) noexcept;
    Path& operator=(Path&& other) noexcept;
   ~Path();

    void Swap(Path& other);

    const std::filesystem::path& NativePath() const;
    void* NativeHandle() const;
    int Size() const;

  protected:
    std::filesystem::path m_path{};
    void* m_addr{};
  };
}

inline bool operator==(const AxonIPC::Path& lhs, const AxonIPC::Path& rhs)
{
  return lhs.NativePath() == rhs.NativePath();
}

inline bool operator!=(const AxonIPC::Path& lhs, const AxonIPC::Path& rhs)
{
  return !(lhs == rhs);
}