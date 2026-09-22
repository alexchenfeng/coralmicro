I've successfully cloned the `v1.10.1` release of `zenoh-pico` into `third_party/zenoh-pico/`. Concurrently, the `CMakeLists.txt` within `libs/zenoh_pico/` has been updated to define `libs_zenoh_pico`, spec...
  Support for Eclipse Zenoh-Pico (v1.10.1 https://github.com/eclipse-zenoh/zenoh-pico) has been integrated into Coral Micro FreeRTOS with complete CMake configurations and a functional publisher example
  application.
  ──────
  ### Summary of Changes
  1. Zenoh-Pico Library:
      • Cloned Zenoh-Pico v1.10.1 into zenoh-pico.
      • Created the library definition in CMakeLists.txt targeting FreeRTOS and lwIP on the Cortex-M7 core (add_library_m7).
      • Configured config.h via configure_file with FreeRTOS multithreading, session management, and TCP client transport enabled.
      • Registered zenoh_pico in CMakeLists.txt.
  2. Publisher Example:
      • Created CMakeLists.txt using add_executable_m7.
      • Created zenoh_pico_publisher.cc which initializes Wi-Fi, creates a Zenoh client session connecting over TCP, declares a publisher for demo/coralmicro/telemetry, and periodically streams JSON sensor
      telemetry (CPU temperature, board uptime, sequence ID) while blinking the user LED.
      • Registered zenoh_pico_publisher in CMakeLists.txt.
  ──────
  ### 1. Zenoh-Pico Library CMake

  CMakeLists.txt:
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

    set(FRAG_MAX_SIZE 4096 CACHE STRING "Override maximum size for fragmented messages")
    set(BATCH_UNICAST_SIZE 2048 CACHE STRING "Override maximum unicast batch size")
    set(BATCH_MULTICAST_SIZE 2048 CACHE STRING "Override maximum multicast batch size")
    set(Z_CONFIG_SOCKET_TIMEOUT 100 CACHE STRING "Default socket timeout in milliseconds")
    set(Z_TRANSPORT_LEASE 10000 CACHE STRING "Link lease duration in milliseconds")
    set(Z_TRANSPORT_LEASE_EXPIRE_FACTOR 3 CACHE STRING "Default session lease expire factor")
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
  ──────
  ### 2. Example Application CMake

  CMakeLists.txt:

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

    add_executable_m7(zenoh_pico_publisher
        zenoh_pico_publisher.cc
    )

    target_link_libraries(zenoh_pico_publisher
        libs_base-m7_freertos
        libs_base-m7_wifi
        libs_zenoh_pico
    )
  ──────
  ### 3. Example FreeRTOS Application

  zenoh_pico_publisher.cc:
    // Copyright 2024 Google LLC
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

    #include <cstdio>
    #include <cstring>

    #include "libs/base/led.h"
    #include "libs/base/tempsense.h"
    #include "libs/base/timer.h"
    #include "libs/base/wifi.h"
    #include "third_party/freertos_kernel/include/FreeRTOS.h"
    #include "third_party/freertos_kernel/include/task.h"
    #include "zenoh-pico.h"

    // Configuration: Point to your Zenoh router (zenohd) IP and port
    #ifndef ZENOH_ROUTER_LOCATOR
    #define ZENOH_ROUTER_LOCATOR "tcp/192.168.1.100:7447"
    #endif

    #ifndef ZENOH_TOPIC_KEYEXPR
    #define ZENOH_TOPIC_KEYEXPR "demo/coralmicro/telemetry"
    #endif

    namespace coralmicro {
    namespace {

    void RunZenohPublisher() {
      printf("=========================================\r\n");
      printf("  Coral Micro Zenoh-Pico FreeRTOS Demo   \r\n");
      printf("=========================================\r\n");

      // Turn on Status LED
      LedSet(Led::kStatus, true);

      // Initialize and connect Wi-Fi
      printf("Turning on Wi-Fi interface...\r\n");
      if (!WiFiTurnOn(/*default_iface=*/true)) {
        printf("ERROR: Failed to initialize Wi-Fi hardware!\r\n");
        vTaskSuspend(nullptr);
      }

      printf("Connecting to Wi-Fi network...\r\n");
      if (!WiFiConnect(10)) {
        printf("ERROR: Failed to connect to Wi-Fi SSID!\r\n");
        printf("Hint: Make sure Wi-Fi credentials are set via flashtool.py or flash config.\r\n");
        vTaskSuspend(nullptr);
      }

      auto ip = WiFiGetIp();
      if (ip.has_value()) {
        printf("Wi-Fi connected successfully! Board IP: %s\r\n", ip->c_str());
      } else {
        printf("ERROR: Failed to obtain IP address from DHCP.\r\n");
        vTaskSuspend(nullptr);
      }

      // Initialize Zenoh-Pico configuration
      printf("Initializing Zenoh-Pico client configuration...\r\n");
      z_owned_config_t config;
      z_config_default(&config);
      zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, "client");
      zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, ZENOH_ROUTER_LOCATOR);

      printf("Connecting to Zenoh router at '%s'...\r\n", ZENOH_ROUTER_LOCATOR);
      z_owned_session_t session;
      if (z_open(&session, z_move(config), nullptr) < 0) {
        printf("ERROR: Failed to open Zenoh session with router at %s!\r\n", ZENOH_ROUTER_LOCATOR);
        printf("Please check that zenohd is running and reachable on your network.\r\n");
        vTaskSuspend(nullptr);
      }
      printf("Zenoh session established!\r\n");

      // Declare Zenoh publisher
      printf("Declaring Zenoh publisher for key expression: '%s'\r\n", ZENOH_TOPIC_KEYEXPR);
      z_owned_publisher_t pub;
      z_view_keyexpr_t ke;
      z_view_keyexpr_from_str_unchecked(&ke, ZENOH_TOPIC_KEYEXPR);
      if (z_declare_publisher(z_loan(session), &pub, z_loan(ke), nullptr) < 0) {
        printf("ERROR: Failed to declare Zenoh publisher!\r\n");
        z_drop(z_move(session));
        vTaskSuspend(nullptr);
      }
      printf("Zenoh publisher declared successfully. Starting telemetry stream...\r\n");

      char msg_buf[192];
      uint32_t seq = 0;
      bool user_led_state = false;

      while (true) {
        float cpu_temp = TempSensorRead(TempSensor::kCpu);
        uint64_t uptime_ms = TimerMillis();

        // Format JSON telemetry payload
        int len = snprintf(msg_buf, sizeof(msg_buf),
                           "{\"seq\":%lu,\"cpu_temp_c\":%.2f,\"uptime_ms\":%llu,\"device\":\"coralmicro\"}",
                           (unsigned long)seq, (double)cpu_temp, (unsigned long long)uptime_ms);

        if (len > 0) {
          z_owned_bytes_t payload;
          z_bytes_copy_from_str(&payload, msg_buf);

          z_publisher_put_options_t options;
          z_publisher_put_options_default(&options);

          z_result_t res = z_publisher_put(z_loan(pub), z_move(payload), &options);
          if (res >= 0) {
            printf("[Pub #%lu] '%s' -> %s\r\n", (unsigned long)seq, ZENOH_TOPIC_KEYEXPR, msg_buf);
            user_led_state = !user_led_state;
            LedSet(Led::kUser, user_led_state);
          } else {
            printf("WARNING: Failed to publish message #%lu (res: %d)\r\n", (unsigned long)seq, (int)res);
          }
        }

        seq++;
        vTaskDelay(pdMS_TO_TICKS(1000));
      }

      // Graceful cleanup
      z_drop(z_move(pub));
      z_drop(z_move(session));
    }

    }  // namespace
    }  // namespace coralmicro

    extern "C" void app_main(void* param) {
      (void)param;
      coralmicro::RunZenohPublisher();
    }
  ──────
  ### 4. Build and Run Instructions

  #### Step 1: Build the Target

  From /opt/coralmicro:

    make -C build zenoh_pico_publisher -j$(nproc)

  (The binary builds to build/examples/zenoh_pico_publisher/zenoh_pico_publisher along with image.hex and image.srec).

  #### Step 2: Flash to Coral Dev Board Micro

  Set your Wi-Fi SSID and password during flashing:

    python3 scripts/flashtool.py \
        -e zenoh_pico_publisher \
        --wifi_ssid "YOUR_WIFI_SSID" \
        --wifi_psk "YOUR_WIFI_PASSWORD"

  #### Step 3: Start the Zenoh Router on Host

  On your computer / server (matching the IP configured in ZENOH_ROUTER_LOCATOR):

    # Using Docker
    docker run --init --net host eclipse/zenoh:latest

    # Or using standalone binary
    cargo install zenoh-cli
    zenohd

  #### Step 4: Subscribe to Telemetry on Host

  To view the live telemetry messages:

    # Using Zenoh CLI
    zenoh sub "demo/coralmicro/**"

    # Or using Python (pip install eclipse-zenoh)
    python3 -c "import zenoh; session = zenoh.open(); sub = session.declare_subscriber('demo/coralmicro/**', lambda s: print(f'{s.key_expr}: {s.payload.to_string()}')); input('Press Enter to exit...\n')"

