#ifndef APPS_RACKTEST_RACK_TEST_IPC_H_
#define APPS_RACKTEST_RACK_TEST_IPC_H_

#include "libs/base/message_buffer.h"

enum class RackTestAppMessageType : uint8_t {
  XOR = 0,
  COREMARK,
};

struct RackTestAppMessage {
  RackTestAppMessageType message_type;
  union {
    uint32_t xor_value;
    char* buffer_ptr;
  } message;
};
static_assert(sizeof(RackTestAppMessage) <=
              coral::micro::ipc::kMessageBufferDataSize);

#endif  // APPS_RACKTEST_RACK_TEST_IPC_H_
