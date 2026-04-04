#include <gtest/gtest.h>
#include "AxonIPC/Publisher.h"
#include "AxonIPC/Subscriber.h"
#include "AxonIPC/Path.h"
#include "ContextMock.h"

using namespace testing;
class AxonIPCTests : public Test
{
public:
  AxonIPCTests()
  {
    EXPECT_CALL(context, CreateSocket()).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(context, LastErrorCode()).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(context, SocketClose(_)).Times(2);
    EXPECT_CALL(context, Shutdown(_)).Times(2);
    EXPECT_CALL(context, Bind(_,_)).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(context, Unbind(_)).Times(1);

    EXPECT_CALL(context, SendTo(_, _, _, _))
      .WillOnce(Invoke([&](AxonIPC::socket_t, const void* mess, const size_t size, const AxonIPC::Path&)
        {
          sent.assign(static_cast<const char*>(mess), size);
          return !sent.empty();
        }));

    EXPECT_CALL(context, RecvFrom(_, _, _, _))
      .WillRepeatedly(Invoke([&](AxonIPC::socket_t, void* mess, const size_t size, size_t& bytesRead)
        {
          if (!sent.empty())
          {
            sent.copy(static_cast<char*>(mess), size);
            sent.erase(0, size);
            bytesRead = size;
            return true;
          }
          return false;
        }));
  }

  ContextMock context;
  std::string sent;
};

TEST_F(AxonIPCTests, AxonIPCSubscribe)
{
  // Arrange
  int type = 0;
  std::string_view publisherPath, payload;

  AxonIPC::Publisher publisher(context, AxonIPC::Path());
  AxonIPC::Subscriber subscriber(context, AxonIPC::Path());

  // Act
  publisher.Publish(42, "Serialize", AxonIPC::Path());
  subscriber.Receive(type, publisherPath, payload);

  // Assert
  EXPECT_EQ(type, 42);
  EXPECT_EQ(payload, "Serialize");
}
