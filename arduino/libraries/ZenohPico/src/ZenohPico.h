// Copyright 2026 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ZENOH_PICO_ARDUINO_H_
#define ZENOH_PICO_ARDUINO_H_

#ifndef ZENOH_FREERTOS_LWIP
#define ZENOH_FREERTOS_LWIP 1
#endif

#ifndef ZENOH_COMPILER_GCC
#define ZENOH_COMPILER_GCC 1
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

#ifndef u32_t
#include <stdint.h>
typedef uint32_t u32_t;
#endif

#include "zenoh-pico.h"

// Undefine lwIP BSD socket macros that collide with Arduino Stream/Serial/Print methods
#ifdef write
#undef write
#endif
#ifdef read
#undef read
#endif
#ifdef close
#undef close
#endif

#endif  // ZENOH_PICO_ARDUINO_H_
