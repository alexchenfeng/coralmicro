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

// Configuration: Adjust router IP and port to point to your Zenoh router (zenohd).
// Example: "tcp/192.168.1.145:7447"
#ifndef ZENOH_ROUTER_LOCATOR
#define ZENOH_ROUTER_LOCATOR "tcp/192.168.1.159:7447"
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

  // Initialize temperature sensor hardware
  TempSensorInit();

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
  int8_t open_res = z_open(&session, z_move(config), nullptr);
  if (open_res < 0) {
    printf("ERROR: Failed to open Zenoh session with router at %s (code: %d)!\r\n",
           ZENOH_ROUTER_LOCATOR, (int)open_res);
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
    uint32_t uptime_ms = static_cast<uint32_t>(TimerMillis());

    // Format JSON telemetry payload
    int len = snprintf(msg_buf, sizeof(msg_buf),
                       "{\"seq\":%lu,\"cpu_temp_c\":%.2f,\"uptime_ms\":%lu,\"device\":\"coralmicro\"}",
                       static_cast<unsigned long>(seq),
                       static_cast<double>(cpu_temp),
                       static_cast<unsigned long>(uptime_ms));

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

  // Graceful cleanup (unreached in infinite loop)
  z_drop(z_move(pub));
  z_drop(z_move(session));
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::RunZenohPublisher();
}
