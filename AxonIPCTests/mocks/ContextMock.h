#pragma once
#include <gmock/gmock.h>
#include "AxonIPC/Context.h"
#include "AxonIPC/Platform.h"
#include "AxonIPC/Path.h"

class ContextMock : public AxonIPC::Context
{
public:
  MOCK_METHOD(int, LastErrorCode, (), (const override));
  MOCK_METHOD(AxonIPC::socket_t, CreateSocket, (), (override));
  MOCK_METHOD(void, SocketClose, (AxonIPC::socket_t), (override));
  MOCK_METHOD(void, Shutdown, (AxonIPC::socket_t), (override));
  MOCK_METHOD(bool, Bind, (AxonIPC::socket_t, const AxonIPC::Path&), (override));
  MOCK_METHOD(void, Unbind, (const AxonIPC::Path&), (override));
  MOCK_METHOD(bool, SendTo, (AxonIPC::socket_t, const void*, size_t, const AxonIPC::Path&), (override));
  MOCK_METHOD(bool, RecvFrom, (AxonIPC::socket_t, void*, size_t, size_t&), (override));
};
