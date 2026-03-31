#include <gtest/gtest.h>
#include <string_view>
#include <array>
#include "AxonIPC/BinaryReader.h"
#include "AxonIPC/BinaryWriter.h"

TEST(BinaryReaderTests, Test)
{
  constexpr unsigned int value = 0;

  std::array<char, 1000> arr{};
  AxonIPC::BinaryWriter writer(arr);
  writer.Write(value);
  writer.Write("Hello World");
  writer.Write(std::string("Goodbye World"));
  writer.Write(std::string_view("Hello Moon"));
  EXPECT_EQ(writer.Size(), 62);

  unsigned int val = 0;
  std::string_view str1, str2, str3;
  AxonIPC::BinaryReader reader(std::span<char>(arr.data(), writer.Size()));

  reader.Read(val);
  reader.Read(str1);
  reader.Read(str2);
  reader.Read(str3);
  EXPECT_EQ(reader.Capacity(), 62);

  EXPECT_EQ(val, value);
  EXPECT_EQ(str1, "Hello World");
  EXPECT_EQ(str2, "Goodbye World");
  EXPECT_EQ(str3, "Hello Moon");
}
