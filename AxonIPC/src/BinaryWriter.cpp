#include "BinaryWriter.h"
#include "Path.h"
#include <cstring>

namespace AxonIPC
{
  BinaryWriter::BinaryWriter(std::span<char>&& data)
    : m_data(data)
  {}

  BinaryWriter::BinaryWriter(BinaryWriter&& other) noexcept
    : m_data(std::move(other.m_data))
    , m_cursor(other.m_cursor)
  {
    other.m_cursor = {};
  }

  BinaryWriter& BinaryWriter::operator=(BinaryWriter&& other) noexcept
  {
    BinaryWriter(std::move(other)).Swap(*this);
    return *this;
  }

  void BinaryWriter::Swap(BinaryWriter& other)
  {
    std::swap(m_data, other.m_data);
    std::swap(m_cursor, other.m_cursor);
  }

  void BinaryWriter::Write(const Path& val)
  {
    Write(val.NativePath().string());
  }

  void BinaryWriter::Write(const std::string_view& val)
  {
    if (m_cursor + val.size() > m_data.size())
      throw std::runtime_error("overflow");

    Write(val.size());

    std::copy(val.begin(), val.end(), &m_data[m_cursor]);
    m_cursor += val.size();
  }

  void BinaryWriter::Write(const std::string& val)
  {
    if (m_cursor + val.size() > m_data.size())
      throw std::runtime_error("overflow");

    Write(val.size());

    std::copy(val.begin(), val.end(), &m_data[m_cursor]);
    m_cursor += val.size();
  }

  void BinaryWriter::Write(const char* val)
  {
    const size_t size = strlen(val);
    if (m_cursor + size > m_data.size())
      throw std::runtime_error("overflow");

    Write(size);

    std::copy(&val[0], &val[size], &m_data[m_cursor]);
    m_cursor += size;
  }

  size_t BinaryWriter::Capacity() const
  {
    return m_data.size();
  }

  size_t BinaryWriter::Size() const
  {
    return m_cursor;
  }
}
