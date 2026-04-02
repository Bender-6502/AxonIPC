#pragma once
#include <string>
#include <string_view>
#include <span>
#include <stdexcept>

namespace AxonIPC
{
  class Path;
  class BinaryReader
  {
  public:
    BinaryReader() = default;
    BinaryReader(std::span<char>&& data);
    BinaryReader(const BinaryReader&) = delete;
    BinaryReader(BinaryReader&& other) noexcept;
    BinaryReader& operator=(const BinaryReader&) = delete;
    BinaryReader& operator=(BinaryReader&& other) noexcept;
   ~BinaryReader() = default;

    void Swap(BinaryReader& other);

    template<class T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    void Read(T& val)
    {
      if (m_cursor + sizeof(T) > m_data.size())
        throw std::runtime_error("overflow");

      std::copy(&m_data[m_cursor], &m_data[m_cursor + sizeof(T)], reinterpret_cast<char*>(&val));
      m_cursor += sizeof(T);
    }

    void Read(Path& val);
    void Read(std::string_view& val);
    void Read(std::string& val);

    size_t Capacity() const;

  private:
    std::span<char> m_data{};
    size_t m_cursor{};
  };
}
