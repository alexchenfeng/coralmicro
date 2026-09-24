# Copyright 2024 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#   Zenoh-Pico Library:
#      • Cloned Zenoh-Pico v1.10.1 into zenoh-pico.
#      • Created the library definition in CMakeLists.txt targeting FreeRTOS and lwIP on the Cortex-M7 core (add_library_m7).
#      • Configured config.h via configure_file with FreeRTOS multithreading, session management, and TCP client transport enabled.
#      • Registered zenoh_pico in CMakeLists.txt.

set(FRAG_MAX_SIZE 4096 CACHE STRING "Override maximum size for fragmented messages")
set(BATCH_UNICAST_SIZE 2048 CACHE STRING "Override maximum unicast batch size")
set(BATCH_MULTICAST_SIZE 2048 CACHE STRING "Override maximum multicast batch size")
# Increased Z_CONFIG_SOCKET_TIMEOUT from 100 ms to 1000 ms to prevent premature timeouts during Wi-Finetwork latency.
set(Z_CONFIG_SOCKET_TIMEOUT 1000 CACHE STRING "Default socket timeout in milliseconds")
set(Z_TRANSPORT_LEASE 120000 CACHE STRING "Link lease duration in milliseconds" FORCE)
set(Z_TRANSPORT_LEASE_EXPIRE_FACTOR 3 CACHE STRING "Default session lease expire factor" FORCE)
set(Z_RUNTIME_MAX_TASKS 64 CACHE STRING "Maximum number of tasks in zenoh-pico runtime")
set(Z_RUNTIME_IDLE_READ_TASK_SLEEP 0 CACHE STRING "Idle read task sleep duration in milliseconds")
set(Z_TRANSPORT_ACCEPT_TIMEOUT 1000 CACHE STRING "Link accept timeout in milliseconds")
set(Z_TRANSPORT_CONNECT_TIMEOUT 10000 CACHE STRING "Link connect timeout in milliseconds")

set(Z_FEATURE_UNSTABLE_API 0)
set(Z_FEATURE_CONNECTIVITY 0)
set(Z_FEATURE_MULTI_THREAD 1)
set(Z_FEATURE_PUBLICATION 1)
set(Z_FEATURE_ADVANCED_PUBLICATION 0)
set(Z_FEATURE_SUBSCRIPTION 1)
set(Z_FEATURE_ADVANCED_SUBSCRIPTION 0)
set(Z_FEATURE_QUERY 1)
set(Z_FEATURE_QUERYABLE 1)
set(Z_FEATURE_LIVELINESS 1)
set(Z_FEATURE_RAWETH_TRANSPORT 0)
set(Z_FEATURE_INTEREST 1)
set(Z_FEATURE_LINK_TCP 1)
set(Z_FEATURE_LINK_BLUETOOTH 0)
set(Z_FEATURE_LINK_WS 0)
set(Z_FEATURE_LINK_SERIAL 0)
set(Z_FEATURE_LINK_SERIAL_USB 0)
set(Z_FEATURE_LINK_TLS 0)
set(Z_FEATURE_SCOUTING 0)
set(Z_FEATURE_LINK_UDP_MULTICAST 0)
set(Z_FEATURE_LINK_UDP_UNICAST 0)
set(Z_FEATURE_MULTICAST_TRANSPORT 0)
set(Z_FEATURE_UNICAST_TRANSPORT 1)
set(Z_FEATURE_FRAGMENTATION 1)
set(Z_FEATURE_ENCODING_VALUES 1)
set(Z_FEATURE_TCP_NODELAY 1)
set(Z_FEATURE_LOCAL_SUBSCRIBER 0)
set(Z_FEATURE_LOCAL_QUERYABLE 0)
set(Z_FEATURE_SESSION_CHECK 1)
set(Z_FEATURE_BATCHING 1)
set(Z_FEATURE_BATCH_TX_MUTEX 0)
set(Z_FEATURE_BATCH_PEER_MUTEX 0)
set(Z_FEATURE_MATCHING 1)
set(Z_FEATURE_RX_CACHE 0)
set(Z_FEATURE_UNICAST_PEER 0)
set(Z_FEATURE_AUTO_RECONNECT 0)
set(Z_FEATURE_MULTICAST_DECLARATIONS 0)
set(Z_FEATURE_ADMIN_SPACE 0)

configure_file(
    ${PROJECT_SOURCE_DIR}/third_party/zenoh-pico/include/zenoh-pico/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/zenoh-pico/config.h
)

set(ZENOH_PICO_DIR ${PROJECT_SOURCE_DIR}/third_party/zenoh-pico)

file(GLOB_RECURSE ZENOH_PICO_SOURCES
    "${ZENOH_PICO_DIR}/src/api/*.c"
    "${ZENOH_PICO_DIR}/src/collections/*.c"
    "${ZENOH_PICO_DIR}/src/net/*.c"
    "${ZENOH_PICO_DIR}/src/protocol/*.c"
    "${ZENOH_PICO_DIR}/src/runtime/*.c"
    "${ZENOH_PICO_DIR}/src/session/*.c"
    "${ZENOH_PICO_DIR}/src/transport/*.c"
    "${ZENOH_PICO_DIR}/src/utils/*.c"
    "${ZENOH_PICO_DIR}/src/system/common/*.c"
)

file(GLOB ZENOH_PICO_LINK_SOURCES
    "${ZENOH_PICO_DIR}/src/link/config/*.c"
    "${ZENOH_PICO_DIR}/src/link/multicast/*.c"
    "${ZENOH_PICO_DIR}/src/link/unicast/*.c"
    "${ZENOH_PICO_DIR}/src/link/transport/common/*.c"
)
list(APPEND ZENOH_PICO_SOURCES ${ZENOH_PICO_LINK_SOURCES})

list(APPEND ZENOH_PICO_SOURCES
    "${ZENOH_PICO_DIR}/src/link/link.c"
    "${ZENOH_PICO_DIR}/src/link/endpoint.c"
    "${ZENOH_PICO_DIR}/src/link/transport/tcp/address.c"
    "${ZENOH_PICO_DIR}/src/link/transport/udp/address.c"
    "${ZENOH_PICO_DIR}/src/link/transport/upper/serial_protocol.c"
    "${ZENOH_PICO_DIR}/src/link/transport/upper/tls_stream.c"
    "${ZENOH_PICO_DIR}/src/system/freertos/system.c"
    "${ZENOH_PICO_DIR}/src/system/socket/lwip.c"
    "${ZENOH_PICO_DIR}/src/link/transport/tcp/tcp_lwip.c"
)

add_library_m7(libs_zenoh_pico STATIC
    ${ZENOH_PICO_SOURCES}
)

target_include_directories(libs_zenoh_pico PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ZENOH_PICO_DIR}/include
)

target_compile_definitions(libs_zenoh_pico PUBLIC
    ZENOH_FREERTOS_LWIP=1
    ZENOH_COMPILER_GCC=1
    INET6_ADDRSTRLEN=46
    u32_t=uint32_t
    Z_TRANSPORT_LEASE=120000
    Z_TRANSPORT_LEASE_EXPIRE_FACTOR=3
)

target_compile_options(libs_zenoh_pico PRIVATE
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-unused-function
)

target_link_libraries(libs_zenoh_pico PUBLIC
    libs_base-m7_freertos
    libs_nxp_rt1176-sdk_lwip
)
