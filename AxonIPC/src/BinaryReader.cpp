#include "BinaryReader.h"

namespace AxonIPC
{
  BinaryReader::BinaryReader(std::span<char>&& data)
    : m_data(data)
    , m_cursor()
  {}

  BinaryReader::BinaryReader(BinaryReader&& other) noexcept
    : m_data(std::move(other.m_data))
    , m_cursor(other.m_cursor)
  {
    other.m_cursor = {};
  }

  BinaryReader& BinaryReader::operator=(BinaryReader&& other) noexcept
  {
    BinaryReader(std::move(other)).Swap(*this);
    return *this;
  }

  void BinaryReader::Swap(BinaryReader& other)
  {
    std::swap(m_data, other.m_data);
    std::swap(m_cursor, other.m_cursor);
  }

  void BinaryReader::Read(std::string_view& val)
  {
    size_t size = 0;
    Read(size);

    if (m_cursor + size > m_data.size())
      throw std::runtime_error("overflow");

    val = std::string_view(&m_data[m_cursor], size);
    m_cursor += size;
  }

  void BinaryReader::Read(std::string& val)
  {
    size_t size = 0;
    Read(size);

    if (m_cursor + size > m_data.size())
      throw std::runtime_error("overflow");

    val = std::string(&m_data[m_cursor], size);
    m_cursor += size;
  }

  size_t BinaryReader::Capacity() const
  {
    return m_data.size();
  }
}
