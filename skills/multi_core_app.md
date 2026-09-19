Create a multi-core app
=======================

The Dev Board Micro has a dual-core MCU with a Cortex M7 core and a Cortex M4
core, enabling a variety of multi-core applications. When the
board boots up, the bootloader starts the M7
executable only. So if you want to use the M4 core, you need to start it
from your M7 program. This page shows you how to do that and introduces
some of the inter-process communication (IPC) features available on the board.

Define an M4 program with CMake
-------------------------------

First thing you need to do is tell CMake which program you want to run on the
M4. You can do that with the following updates to your `CMakeLists.txt`:

* Declare the M4 executable with `add_executable_m4()`. (This works
  exactly the same as `add_executable_m7()` because they're both wrappers for
  [`add_executable()`](https://cmake.org/cmake/help/latest/command/add_executable.html).)
* Link the M4 executable's libraries with
  [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).
* Add the M4 executable target to the M7 executable by declaring
  it with the `M4_EXECUTABLE` property. (The M4 executable will be bundled into
  the M7 binary.)

For example, the following `CMakeLists.txt` declares
an M7 executable named `multi_core_hello`, which includes the M4 executable
named `multi_core_hello_m4`:

```
add_executable_m7(multi_core_hello
    hello_world_cm7.cc
    M4_EXECUTABLE
    multi_core_hello_m4
)

target_link_libraries(multi_core_hello
    libs_base-m7_freertos
)

add_executable_m4(multi_core_hello_m4
    hello_world_cm4.cc
)

target_link_libraries(multi_core_hello_m4
    libs_base-m4_freertos
)
```

Notice that the linked libraries for each executable are unique, because
not all libraries are the same for the M7 and M4.

Execute the M4 program
----------------------

Just like an M7 program, an M4 program must include an `app_main()` function as
its entry point. So your code for the M4 might appear identical to code that
runs on the M7. However, only the M7 program's `app_main()` starts automatically
upon boot.

When the board boots, the bootloader loads the M4 program into memory but it
does not run until you start it from your M7 program by calling
[`IpcM7::StartM4()`](/docs/reference/micro/system/#_CPPv4N10coralmicro5IpcM77StartM4Ev).
For example, the following M7 program (`hello_world_cm7.cc` from the above
CMake file) starts the M4 program (`hello_world_cm4.cc`)
and then suspends itself:

```
#include "libs/base/ipc_m7.h"
#include "libs/base/mutex.h"

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::IpcM7::GetSingleton()->StartM4();
  CHECK(coralmicro::IpcM7::GetSingleton()->M4IsAlive(500));
  vTaskSuspend(nullptr);
}
```

**Note:** Just like there can be only one M7 executable
`app_main()`, there's only one M4 executable, so you don't need
to specify its name. Just start the M4 and the system invokes
`app_main()` in your M4 executable.

### Caveats

Although most code works the same on each core, there are some
important exceptions:

* **M4 can't use the Edge TPU:** Only the M7 can execute inference on the Edge
  TPU. This is because the M4 currently cannot access the USB stack. (You can
  still [run TFLite Micro models on the
  M4](/docs/reference/micro/tensorflow/).)
* **M4 must initialize resources:** Many system resources are initialized by
  default on the M7, but not on the M4. For
  example, the camera is already initialized for M7 programs, but an M4 program
  must initialize the camera before you can use it. For example:

  ```
  // Only M4 programs need to call camera's Init()
  CameraTask::GetSingleton()->Init(I2C5Handle());
  CameraTask::GetSingleton()->SetPower(false);
  ```

  Other resources that must be initialized for the M4 include
  [timers](/docs/reference/micro/utils/#timers), [random
  number generators](/docs/reference/micro/utils/#random-numbers), and
  [temperature sensors](/docs/reference/micro/utils/#temperature-sensors).

Also beware that there is no internal arbitration if both cores attempt to
access a shared resource such as the camera. If each core wants to
use the camera (or other shared resource), you must use a
[`MulticoreMutexLock`](/docs/reference/micro/system/#mutex) to avoid both cores
calling upon it at the same time, which can have undefined results.

Communicate between cores
-------------------------

There are essentially two ways to communicate between the M7 and M4: with
a mutex lock to avoid simultaneous access to a shared resource, and with
inter-process communication (IPC) to send custom app messages.

To protect a shared resource with a mutex lock that works across cores, simply
create a [`MulticoreMutexLock`](/docs/reference/micro/system/#mutex) on each
core, using the same gate number. This lock should be initialized right before
each program attempts to use a shared resource, so that code will be blocked in
the event that the other core still holds the lock. The lock is released by each
core once the code leaves the scope of the
[`MulticoreMutexLock`](/docs/reference/micro/system/#mutex) object.

To pass messages between cores, you just need to create a message with
`IpcMessage` and, depending whether your sender is on the M7 or M4, send it
to the other process with either `IpcM7::SendMessage()` or
`IpcM4::SendMessage()`, respectively. For details, see the
[IPC API reference](/docs/reference/micro/system/#inter-process-communication-ipc).

Multi-core examples
-------------------

Sometimes, the best way to learn is to read some working code. So check out
these examples that use both MCU cores:

* [multi\_core\_blink\_led](https://github.com/google-coral/coralmicro/blob/main/examples/multi_core_blink_led/):
  Starts an M4 app to blink an LED and suspends the M7 app.
* [multi\_core\_hello](https://github.com/google-coral/coralmicro/blob/main/examples/multi_core_hello/):
  Starts an M4 app and then continuously prints "Hello world" from each core
  (using a mutex lock to ensure alternating messages).
* [multi\_core\_ipc](https://github.com/google-coral/coralmicro/blob/main/examples/multi_core_ipc/):
  Performs IPC between cores, with each core sending and receiving messages