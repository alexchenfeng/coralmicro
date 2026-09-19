System APIs
===========

APIs to manage system processes.

Inter-processor communication (IPC)
-----------------------------------

APIs to initiate multicore processing and pass messages between the M7 and M4
cores.

For example, the following code (from `examples/multi_core_ipc/`) executes on
the M7. It registers a handler to receive messages from the M4 program
right before it starts the M4. Then it starts a loop to also periodically
send messages to the M4:

```
namespace coralmicro {
namespace {

void HandleM4Message(const uint8_t data[kIpcMessageBufferDataSize]) {
  const auto* msg = reinterpret_cast<const ExampleAppMessage*>(data);
  if (msg->type == ExampleMessageType::kAck) {
    printf("[M7] ACK received from M4\r\n");
  }
}

[[noreturn]] void Main() {
  printf("Multicore IPC Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  auto* ipc = IpcM7::GetSingleton();
  ipc->RegisterAppMessageHandler(HandleM4Message);
  ipc->StartM4();
  CHECK(ipc->M4IsAlive(500));

  bool led_status = false;
  while (true) {
    led_status = !led_status;
    printf("---\r\n[M7] Sending M4 LED_STATUS: %s\r\n",
           led_status ? "ON" : "OFF");

    IpcMessage msg{};
    msg.type = IpcMessageType::kApp;
    auto* app_msg = reinterpret_cast<ExampleAppMessage*>(&msg.message.data);
    app_msg->type = ExampleMessageType::kLedStatus;
    app_msg->led_status = led_status;
    ipc->SendMessage(msg);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

}  // namespace
}  // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param) {
  (void)param;
  coralmicro::Main();
}
```

And the following is the counterpart code that runs on the M4. It registers
a message handler to receive incoming messages from the M7, which simply
passes another message back to the M7:

```
namespace coralmicro {
namespace {

void HandleM7Message(const uint8_t data[kIpcMessageBufferDataSize]) {
  const auto* msg = reinterpret_cast<const ExampleAppMessage*>(data);
  if (msg->type == ExampleMessageType::kLedStatus) {
    printf("[M4] LED_STATUS received: %s\r\n", msg->led_status ? "ON" : "OFF");
    LedSet(Led::kUser, msg->led_status);

    IpcMessage ack_msg{};
    ack_msg.type = IpcMessageType::kApp;
    auto* app_msg = reinterpret_cast<ExampleAppMessage*>(&ack_msg.message.data);
    app_msg->type = ExampleMessageType::kAck;
    IpcM4::GetSingleton()->SendMessage(ack_msg);
  }
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  printf("M4 started.\r\n");

  coralmicro::IpcM4::GetSingleton()->RegisterAppMessageHandler(
      coralmicro::HandleM7Message);
  vTaskSuspend(nullptr);
}
```

Notice that the data for `IpcMessage` uses a custom message
format, which is defined in `examples/multi_core_ipc/example_message.h` like
this:

```
enum class ExampleMessageType : uint8_t {
  kLedStatus,
  kAck,
};

struct ExampleAppMessage {
  ExampleMessageType type;
  bool led_status;
} __attribute__((packed));
```

For information about how to get started with multicore processing with the M4,
see the guide to [create a multicore app](/docs/dev-board-micro/multicore/).

*namespace* `coralmicro`
:   *class* `Ipc`[Â¶](#_CPPv4N10coralmicro3IpcE "Permalink to this definition")
    :   *#include <ipc.h>*

        Do not instantiate this class. It provides shared IPC functions for `IpcM7` and `IpcM4`.

        Public Functions

        void `SendMessage`(*const* [IpcMessage](#_CPPv4N10coralmicro10IpcMessageE "coralmicro::IpcMessage") &*message*)[Â¶](#_CPPv4N10coralmicro3Ipc11SendMessageERK10IpcMessage "Permalink to this definition")
        :   Sends an IPC message to the other core.

            Parameters
            :   **message** â The message to send.

        *inline* void `RegisterAppMessageHandler`([AppMessageHandler](#_CPPv4N10coralmicro3Ipc17AppMessageHandlerE "coralmicro::Ipc::AppMessageHandler") *handler*)[Â¶](#_CPPv4N10coralmicro3Ipc25RegisterAppMessageHandlerE17AppMessageHandler "Permalink to this definition")
        :   Sets a callback function to process incoming IPC messages.

            Parameters
            :   **handler** â The function to receive incoming messages.

        Public Types

        *using* `AppMessageHandler` = std::function<void(*const* uint8\_t data[kIpcMessageBufferDataSize])>[Â¶](#_CPPv4N10coralmicro3Ipc17AppMessageHandlerE "Permalink to this definition")
        :   The function type to handle incoming IPC messages, which must be given to `RegisterAppMessageHandler()` via `IpcM7` or `IpcM4`, respective of which core is receiving the messages.

            The function receives the IPC message as a byte array of size `kIpcMessageBufferDataSize` (127).

[[ipc\_m7.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/ipc_m7.h)

*namespace* `coralmicro`
:   *class* `IpcM7` : *public* [coralmicro](../arduino/#_CPPv410coralmicro "coralmicro")::[Ipc](#_CPPv4N10coralmicro3IpcE "coralmicro::Ipc")[Â¶](#_CPPv4N10coralmicro5IpcM7E "Permalink to this definition")
    :   *#include <ipc\_m7.h>*

        Singleton object that provides IPC on the M7 core so it can start the M4 core and relay messages with the M4.

        The M4 can not operate independent from the M7: the M7 must start the M4 with `StartM4()`, and then the M7 task may suspend itself.

        The M7 can send messages to the M4 with `SendMessage()` and register a callback to receive messages from the M4 with `RegisterAppMessageHandler()`.

        Public Functions

        void `StartM4`()[Â¶](#_CPPv4N10coralmicro5IpcM77StartM4Ev "Permalink to this definition")
        :   Starts the M4 core, invoking the `app_main()` function in the executable declared with the CMake `add_executable_m4()` command.

        bool `M4IsAlive`(uint32\_t *millis*)[Â¶](#_CPPv4N10coralmicro5IpcM79M4IsAliveE8uint32_t "Permalink to this definition")
        :   Checks if the M4 core is alive.

            Parameters
            :   **millis** â The amount of time (in milliseconds) to wait for a response from the M4 core.

            Returns
            :   True if the M4 core signals that it is ready to preform a task or preforming a task within the millis time limit, false otherwise.

        Public Static Functions

        *static* *inline* [IpcM7](#_CPPv4N10coralmicro5IpcM7E "coralmicro::IpcM7") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro5IpcM712GetSingletonEv "Permalink to this definition")
        :   Gets the `IpcM7` singleton that can start the M4 and perform IPC with the M4.

            Returns
            :   A reference to the singleton `IpcM7` object.

        *static* bool `HasM4Application`()[Â¶](#_CPPv4N10coralmicro5IpcM716HasM4ApplicationEv "Permalink to this definition")
        :   Checks if the M4 core is running a process.

            Returns
            :   True if the M4 is running a process, false otherwise.

[[ipc\_m4.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/ipc_m4.h)

*namespace* `coralmicro`
:   *class* `IpcM4` : *public* [coralmicro](../arduino/#_CPPv410coralmicro "coralmicro")::[Ipc](#_CPPv4N10coralmicro3IpcE "coralmicro::Ipc")[Â¶](#_CPPv4N10coralmicro5IpcM4E "Permalink to this definition")
    :   *#include <ipc\_m4.h>*

        Singleton object that provides IPC on the M4 core to relay messages with the M7 core.

        The M4 can not operate independent from the M7: the M7 must start the M4 with `IpcM7::StartM4()`, and then the M7 task may suspend itself.

        The M4 can send messages to the M7 with `SendMessage()` and register a callback to receive messages from the M7 with `RegisterAppMessageHandler()`.

        Public Static Functions

        *static* *inline* [IpcM4](#_CPPv4N10coralmicro5IpcM4E "coralmicro::IpcM4") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro5IpcM412GetSingletonEv "Permalink to this definition")
        :   Gets the `IpcM4` singleton to use for IPC with the M7.

            Returns
            :   A reference to the singleton `IpcM4` object.

[[ipc\_message\_buffer.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/ipc_message_buffer.h)

*namespace* `coralmicro`
:   *struct* `IpcMessage`[Â¶](#_CPPv4N10coralmicro10IpcMessageE "Permalink to this definition")
    :   *#include <ipc\_message\_buffer.h>*

        A message to be sent with `Ipc::SendMessage()` (using either `IpcM4` or `IpcM7`).

        The `message` union is designed to support two types of message, but you should always use an âappâ message. So you should set `type` to `IpcMessageType::kApp` and then populate `data` with a custom data format that both processes know how to read/write.

        For an example, see `examples/multi_core_ipc/`.

        Public Members

        [IpcMessageType](#_CPPv4N10coralmicro14IpcMessageTypeE "coralmicro::IpcMessageType") `type`[Â¶](#_CPPv4N10coralmicro10IpcMessage4typeE "Permalink to this definition")
        :   Identifier for the type of message (apps should always use `kApp`).

        IpcSystemMessage `system`[Â¶](#_CPPv4N10coralmicro10IpcMessage6systemE "Permalink to this definition")
        :   Internal use only.

        uint8\_t `data`[`kIpcMessageBufferDataSize`][Â¶](#_CPPv4N10coralmicro10IpcMessage4dataE "Permalink to this definition")
        :   A byte array, which should be a structured data format thatâs defined by the app, but limited to size `kIpcMessageBufferDataSize` (127 bytes).

        *union* [coralmicro](../arduino/#_CPPv410coralmicro "coralmicro")::[IpcMessage](#_CPPv4N10coralmicro10IpcMessageE "coralmicro::IpcMessage")::**[anonymous]** `message`[Â¶](#_CPPv4N10coralmicro10IpcMessage7messageE "Permalink to this definition")
        :   The message to be sent (`system` or `data`).

    Functions

    `struct coralmicro::IpcMessage __attribute__ ((packed))`

    Enums

    *enum class* `IpcMessageType` : uint8\_t[Â¶](#_CPPv4N10coralmicro14IpcMessageTypeE "Permalink to this definition")
    :   The types of message that may be sent in an `IpcMessage`.

        *Values:*

        *enumerator* `kSystem`[Â¶](#_CPPv4N10coralmicro14IpcMessageType7kSystemE "Permalink to this definition")
        :   Internal use only.

        *enumerator* `kApp`[Â¶](#_CPPv4N10coralmicro14IpcMessageType4kAppE "Permalink to this definition")
        :   A custom app message with a byte array of size `kIpcMessageBufferDataSize` (127).

Mutex
-----

APIs to ensure mutual-exclusive access to resources.

[[mutex.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/mutex.h)

*namespace* `coralmicro`
:   *class* `MulticoreMutexLock`[Â¶](#_CPPv4N10coralmicro18MulticoreMutexLockE "Permalink to this definition")
    :   *#include <mutex.h>*

        Defines a mutex lock that is unique across MCU cores, ensuring safe handling of any resources that are shared between the M7 and M4 cores.

        Public Functions

        *inline* *explicit* `MulticoreMutexLock`(uint8\_t *gate*)[Â¶](#_CPPv4N10coralmicro18MulticoreMutexLock18MulticoreMutexLockE7uint8_t "Permalink to this definition")
        :   Acquires a multi-core mutex lock using a semaphore gate number.

            Any code following this request for the mutex lock is blocked until the lock is successfully acquired, and each unique mutex lock can be held by only one task at a time, regardless of which MCU core the task is on. Thus, code-blocks that use the same gate to get a `MulticoreMutexLock` will have thread-safe variables, even across cores.

            After execution leaves the code block where the lock is acquired, the mutex lock is automatically released.

            Parameters
            :   **gate** â An integer representing a unique semaphore gate. Must be within range of the maximum number of gates available on the hardware (16).

    *class* `MutexLock`[Â¶](#_CPPv4N10coralmicro9MutexLockE "Permalink to this definition")
    :   *#include <mutex.h>*

        Defines a mutex lock for the active MCU core, ensuring safe handling of any resources that are shared between tasks.

        Public Functions

        *inline* *explicit* `MutexLock`(SemaphoreHandle\_t *sema*)[Â¶](#_CPPv4N10coralmicro9MutexLock9MutexLockE17SemaphoreHandle_t "Permalink to this definition")
        :   Acquires a mutex lock using a semaphore.

            Any code following this request for the mutex lock is blocked until the lock is successfully acquired, and each unique mutex lock can be held by only one task at a time (on the same MCU core). Thus, code-blocks that use the same semaphore to get a `MutexLock` (on the same MCU core) will have thread-safe variables between said code-blocks.

            After execution leaves the code block where the lock is acquired, the mutex lock is automatically released.

            Parameters
            :   **sema** â The [SemaphoreHandle](https://www.freertos.org/xSemaphoreCreateBinary.html) to define a unique mutex lock.

Watchdog
--------

APIs to create watchdog timers that monitor the MCU behavior and reset it when
it appears to be malfunctioning.

[[watchdog.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/watchdog.h)

*namespace* `coralmicro`
:   *struct* `WatchdogConfig`[Â¶](#_CPPv4N10coralmicro14WatchdogConfigE "Permalink to this definition")
    :   *#include <watchdog.h>*

        Represents a watchdog config, this config needs to be passed into `WatchdogStart()`.

        Public Members

        int `timeout_s`[Â¶](#_CPPv4N10coralmicro14WatchdogConfig9timeout_sE "Permalink to this definition")
        :   Number of seconds before the watchdog resets the CPU (unless pet).

        int `pet_rate_s`[Â¶](#_CPPv4N10coralmicro14WatchdogConfig10pet_rate_sE "Permalink to this definition")
        :   The rate to refresh the watchdog timer in seconds.

        bool `enable_irq`[Â¶](#_CPPv4N10coralmicro14WatchdogConfig10enable_irqE "Permalink to this definition")
        :   Set to true to enable the watchdog interrupt. If watchdog interrupt is enabled, be sure to implement and extern WDOG1\_IRQHandler. Otherwise, you will call DefaultISR.

        int `irq_s_before_timeout`[Â¶](#_CPPv4N10coralmicro14WatchdogConfig20irq_s_before_timeoutE "Permalink to this definition")
        :   When enable\_irq is set, time remaining before watchdog expiration that will trigger an interrupt (e.g. timeout\_s = 10, irq\_s\_before\_timeout=2 means the interrupt will fire at 8 seconds).

    Functions

    void `WatchdogStart`(*const* [WatchdogConfig](#_CPPv4N10coralmicro14WatchdogConfigE "coralmicro::WatchdogConfig") &*config*)[Â¶](#_CPPv4N10coralmicro13WatchdogStartERK14WatchdogConfig "Permalink to this definition")
    :   Enables watchdog and starts SW timer to refresh at a given rate.

        Parameters
        :   **config** â The watchdog config to enable watchdog.

    void `WatchdogStop`()[Â¶](#_CPPv4N10coralmicro12WatchdogStopEv "Permalink to this definition")
    :   Stops SW refresh timer, disables watchdog.

Reset
-----

APIs to reset the Dev Board Micro into different states and read reset
stats.

[[reset.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/reset.h)

*namespace* `coralmicro`
:   *struct* `ResetStats`[Â¶](#_CPPv4N10coralmicro10ResetStatsE "Permalink to this definition")
    :   *#include <reset.h>*

        Represents reset stats.

        Public Members

        uint32\_t `reset_reason`[Â¶](#_CPPv4N10coralmicro10ResetStats12reset_reasonE "Permalink to this definition")
        :   Reset reason, could hold kSRC\_M7CoreWdogResetFlag or kSRC\_M7CoreM7LockUpResetFlag.

        uint32\_t `watchdog_resets`[Â¶](#_CPPv4N10coralmicro10ResetStats15watchdog_resetsE "Permalink to this definition")
        :   Number of watchdog resets.

        uint32\_t `lockup_resets`[Â¶](#_CPPv4N10coralmicro10ResetStats13lockup_resetsE "Permalink to this definition")
        :   Number of lockup resets.

    Functions

    void `ResetToBootloader`()[Â¶](#_CPPv4N10coralmicro17ResetToBootloaderEv "Permalink to this definition")
    :   Reset the board to bootloader mode.

    void `ResetToFlash`()[Â¶](#_CPPv4N10coralmicro12ResetToFlashEv "Permalink to this definition")
    :   Reset the board to flash mode.

    void `ResetStoreStats`()[Â¶](#_CPPv4N10coralmicro15ResetStoreStatsEv "Permalink to this definition")
    :   Stores the current reset stats.

    [ResetStats](#_CPPv4N10coralmicro10ResetStatsE "coralmicro::ResetStats") `ResetGetStats`()[Â¶](#_CPPv4N10coralmicro13ResetGetStatsEv "Permalink to this definition")
    :   Gets the current reset stats.