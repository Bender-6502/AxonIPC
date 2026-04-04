#include <gtest/gtest.h>
#include <future>
#include <condition_variable>
#include <mutex>
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
          std::unique_lock<std::mutex> lck(mtx);
          sendToString.assign(static_cast<const char*>(mess), size);
          ready = true;
          cv.notify_all();
          return ready;
        }));

    EXPECT_CALL(context, RecvFrom(_, _, _, _))
      .WillRepeatedly(Invoke([&](AxonIPC::socket_t, void* mess, const size_t size, size_t& bytesRead)
        {
          // On first call this will wait indefinitely until ready=true, and assuming the sendToString has data will be
          // copied out, and return true.
          //
          // On second entry it will no longer wait, and return false if sendToString is empty, allowing the underlying
          // functionality to exit the worker thread.
          std::unique_lock<std::mutex> lck(mtx);
          while (!ready) cv.wait(lck);

          bytesRead = sendToString.size();
          if (bytesRead)
          {
            sendToString.copy(static_cast<char*>(mess), bytesRead);
            sendToString.clear();
            return true;
          }
          return false;
        }));
  }

  std::condition_variable cv;
  std::mutex mtx;
  ContextMock context;
  std::string sendToString;
  bool ready = false;
};

TEST_F(AxonIPCSubscriberTests, Test)
{
  std::promise<std::string> promise;
  auto future = promise.get_future();

  AxonIPC::AxonIPCSubscriber subscriber(context, AxonIPC::Path("east"));
  subscriber.GetDispatcher()->RegisterSubscriber(42, [&](const int type, const std::string_view& publisher, const std::string_view& payload)
    {
      EXPECT_EQ(publisher, "west");
      promise.set_value(std::string(payload.data(), payload.size()));
    });

  AxonIPC::AxonIPCPublisher publisher(context, AxonIPC::Path("west"));
  publisher.Publish(42, "payload", AxonIPC::Path("east"));

  ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
  EXPECT_EQ(future.get(), "payload");
}
