#include <gtest/gtest.h>
#include <future>
#include "ContextMock.h"
#include "AxonIPC/AxonIPCSubscriber.h"
#include "AxonIPC/AxonIPCPublisher.h"
#include "AxonIPC/Path.h"

using namespace testing;
class AxonIPCSubscriberTests : public Test
{
public:
  AxonIPCSubscriberTests()
  {
    EXPECT_CALL(context, CreateSocket()).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(context, LastErrorCode()).WillRepeatedly(testing::Return(0));
    EXPECT_CALL(context, SocketClose(_)).Times(2);
    EXPECT_CALL(context, Shutdown(_)).Times(2);
    EXPECT_CALL(context, Bind(_, _)).WillRepeatedly(testing::Return(true));
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
            bytesRead = sent.size();
            sent.copy(static_cast<char*>(mess), bytesRead);
            sent.erase(0, bytesRead);

            return true;
          }
          return false;
        }));
  }

  ContextMock context;
  std::string sent;
};

TEST_F(AxonIPCSubscriberTests, Test)
{
  std::promise<std::string> promise;
  auto future = promise.get_future();

  AxonIPC::AxonIPCSubscriber subscriber(context, AxonIPC::Path());
  subscriber.GetDispatcher()->RegisterSubscriber(42, [&](const int type, const std::string_view& payload)
    {
      std::string p;
      std::copy(payload.begin(), payload.end(), std::back_inserter(p));
      promise.set_value(p);
    });

  AxonIPC::AxonIPCPublisher publisher(context, AxonIPC::Path());
  publisher.Publish(42, "payload");

  ASSERT_EQ(future.wait_for(std::chrono::seconds(1)), std::future_status::ready);
  EXPECT_EQ(future.get(), "payload");
}
