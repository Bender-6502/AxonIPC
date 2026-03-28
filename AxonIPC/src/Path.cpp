#include "Path.h"
#include "Platform.h"

namespace AxonIPC
{
  Path::Path(const std::filesystem::path& path)
    : m_path(path)
    , m_addr(CreatePathToAddr(m_path.string()))
  {}

  Path::Path(const Path& other) noexcept
    : m_path(other.m_path)
    , m_addr(CreatePathToAddr(other.m_path))
  {}

  Path::Path(Path&& other) noexcept
    : m_path(std::move(other.m_path))
    , m_addr(other.m_addr)
  {
    other.m_addr = nullptr;
  }

  Path& Path::operator=(const Path& other) noexcept
  {
    Path(other).Swap(*this);
    return *this;
  }

  Path& Path::operator=(Path&& other) noexcept
  {
    Path(std::move(other)).Swap(*this);
    return *this;
  }

  Path::~Path()
  {
    FreePathToAddr(m_addr);
  }

  void Path::Swap(Path& other)
  {
    std::swap(m_path, other.m_path);
    std::swap(m_addr, other.m_addr);
  }

  const std::filesystem::path& Path::NativePath() const
  {
    return m_path;
  }

  void* Path::NativeHandle() const
  {
    return m_addr;
  }

  int Path::Size() const
  {
    return PathToAddrSize();
  }
}
