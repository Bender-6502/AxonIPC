#pragma once
#include <span>
#include <string>
#include <string_view>
#include <stdexcept>

namespace AxonIPC
{
  class Path;
  class BinaryWriter
  {
  public:
    BinaryWriter() = default;
    BinaryWriter(std::span<char>&& data);
    BinaryWriter(const BinaryWriter&) = delete;
    BinaryWriter(BinaryWriter&& other) noexcept;
    BinaryWriter& operator=(const BinaryWriter&) = delete;
    BinaryWriter& operator=(BinaryWriter&& other) noexcept;
   ~BinaryWriter() = default;

    void Swap(BinaryWriter& other);

    template<class T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    void Write(const T& val)
    {
      if (m_cursor + sizeof(T) > m_data.size())
        throw std::runtime_error("overflow");

      std::copy(&reinterpret_cast<const char*>(&val)[0], &reinterpret_cast<const char*>(&val)[sizeof(T)], &m_data[m_cursor]);
      m_cursor += sizeof(T);
    }

    void Write(const Path& val);
    void Write(const std::string_view& val);
    void Write(const std::string& val);
    void Write(const char* val);

    size_t Capacity() const;
    size_t Size() const;

  private:
    std::span<char> m_data{};
    size_t m_cursor{};
  };
}