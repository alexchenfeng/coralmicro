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

#include "picoros.h"
#include "picoserdes.h"

// Configuration: Adjust locator to point to your Zenoh router (running rmw_zenoh or zenohd).
// Example: "tcp/192.168.1.105:7447"
#ifndef ZENOH_ROUTER_LOCATOR
#define ZENOH_ROUTER_LOCATOR "tcp/192.168.1.105:7447"
#endif

#ifndef ROS_DOMAIN_ID
#define ROS_DOMAIN_ID 0
#endif

#ifndef ROS_TOPIC_NAME
#define ROS_TOPIC_NAME "coralmicro/chatter"
#endif

#ifndef ROS_NODE_NAME
#define ROS_NODE_NAME "coralmicro_node"
#endif

namespace coralmicro {
namespace {

// ROS 2 Node configuration
static picoros_node_t s_node = {
    .name = ROS_NODE_NAME,
    .domain_id = ROS_DOMAIN_ID,
};

// ROS 2 Publisher configuration for std_msgs/msg/String
static picoros_publisher_t s_publisher = {
    .topic = {
        .name = const_cast<char*>(ROS_TOPIC_NAME),
        .type = ROSTYPE_NAME(ros_String),
        .rihs_hash = ROSTYPE_HASH(ros_String),
    },
};

// Buffer for CDR serialization
static uint8_t s_pub_buf[512];

bool IsNetworkReady() {
  if (!WiFiIsConnected()) {
    return false;
  }
  auto ip = WiFiGetIp();
  if (!ip.has_value() || ip->empty() || *ip == "0.0.0.0") {
    return false;
  }
  return true;
}

bool EnsureWiFiConnected(int max_retries = 10) {
  if (IsNetworkReady()) {
    return true;
  }
  printf("[reconnect] Network or IP link down! Re-associating with Wi-Fi AP...\r\n");
  int attempts = 1;
  while (attempts <= max_retries) {
    if (WiFiConnect(3)) {
      auto ip = WiFiGetIp();
      if (ip.has_value() && !ip->empty() && *ip != "0.0.0.0") {
        printf("[reconnect] Wi-Fi re-established! IP: %s\r\n", ip->c_str());
        return true;
      }
    }
    printf("[reconnect] Wi-Fi reconnect attempt %d failed, retrying in 3s...\r\n", attempts++);
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
  return false;
}

void RunPicoRosPublisher() {
  printf("=========================================\r\n");
  printf("  Coral Micro Pico-ROS FreeRTOS Demo     \r\n");
  printf("=========================================\r\n");

  // Turn on Status LED indicating system is alive
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

  // Configure Pico-ROS network interface (Zenoh client mode)
  picoros_interface_t ifx = {
      .mode = const_cast<char*>("client"),
      .locator = const_cast<char*>(ZENOH_ROUTER_LOCATOR),
  };

  printf("Connecting Pico-ROS to Zenoh router at '%s'...\r\n", ZENOH_ROUTER_LOCATOR);
  int retry_count = 0;
  while (picoros_interface_init(&ifx) != PICOROS_OK) {
    retry_count++;
    printf("Waiting for Zenoh router at %s... (attempt %d)\r\n", ZENOH_ROUTER_LOCATOR, retry_count);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
  printf("Pico-ROS network interface established!\r\n");

  // Initialize ROS 2 Node
  printf("Initializing ROS 2 node '%s' (Domain ID: %lu)...\r\n",
         s_node.name, static_cast<unsigned long>(s_node.domain_id));
  if (picoros_node_init(&s_node) != PICOROS_OK) {
    printf("ERROR: Failed to initialize ROS 2 node!\r\n");
    picoros_interface_close();
    vTaskSuspend(nullptr);
  }

  // Declare ROS 2 Publisher
  printf("Declaring publisher on topic '%s' [type: %s]...\r\n",
         s_publisher.topic.name, s_publisher.topic.type);
  if (picoros_publisher_declare(&s_node, &s_publisher) != PICOROS_OK) {
    printf("ERROR: Failed to declare publisher on topic '%s'!\r\n", s_publisher.topic.name);
    picoros_node_drop(&s_node);
    picoros_interface_close();
    vTaskSuspend(nullptr);
  }

  printf("Pico-ROS publisher ready. Publishing telemetry every 1 second...\r\n");
  printf("ROS 2 Topic: %s\r\n", s_publisher.topic.name);
  printf("Echo command: ros2 topic echo /%s\r\n", s_publisher.topic.name);

  char msg_text[256];
  uint32_t seq = 0;
  bool user_led_state = false;


  while (true) {
    // Check connection health; reconnect if disconnected
    if (!picoros_interface_is_up()) {
      printf("\r\n[reconnect] WARNING: Pico-ROS connection lost! Reconnecting to %s...\r\n", ZENOH_ROUTER_LOCATOR);
      picoros_publisher_drop(&s_publisher);
      picoros_node_drop(&s_node);
      picoros_interface_close();

      // Check and recover Wi-Fi and IP address
      EnsureWiFiConnected();

      int pico_attempts = 1;
      while (true) {
        EnsureWiFiConnected();

        printf("[reconnect] Connecting to Zenoh router (%s) [attempt %d]...\r\n",
               ZENOH_ROUTER_LOCATOR, pico_attempts++);
        if (picoros_interface_init(&ifx) == PICOROS_OK) {
          printf("[reconnect] Zenoh session connected successfully!\r\n");
          break;
        }
        printf("[reconnect] Zenoh connect failed, retrying in 3s...\r\n");
        vTaskDelay(pdMS_TO_TICKS(3000));

        // Periodically verify Wi-Fi and IP address during retries
        if (!IsNetworkReady()) {
          printf("[reconnect] Wi-Fi link lost or IP expired during retries! Re-associating...\r\n");
          EnsureWiFiConnected();
        }

        // If Zenoh router is unreachable for several attempts, provide diagnostics
        if (pico_attempts == 10) {
          auto cur_ip = WiFiGetIp();
          printf("\r\n[reconnect] DIAGNOSTIC: Board IP is %s. Router %s has not responded for 10 attempts.\r\n"
                 "  Please check:\r\n"
                 "  1) Is the Zenoh router/daemon running on %s?\r\n"
                 "  2) Did the host machine IP change or enter sleep/standby mode?\r\n"
                 "  3) Is port 7447 accessible without firewall blocks?\r\n\r\n",
                 cur_ip.has_value() ? cur_ip->c_str() : "unknown",
                 ZENOH_ROUTER_LOCATOR, ZENOH_ROUTER_LOCATOR);
        }

        // After every 20 failed attempts, perform a full Wi-Fi and DHCP reset to clear any stale lwIP socket/ARP state
        if (pico_attempts % 20 == 0) {
          printf("[reconnect] Refreshing Wi-Fi association and DHCP lease...\r\n");
          WiFiDisconnect();
          EnsureWiFiConnected();
        }
      }

      printf("[reconnect] Re-declaring node and publisher...\r\n");
      picoros_node_init(&s_node);
      picoros_publisher_declare(&s_node, &s_publisher);
      printf("Pico-ROS reconnected successfully!\r\n\r\n");
    }

    // Collect telemetry from hardware sensors
    float cpu_temp = TempSensorRead(TempSensor::kCpu);
    uint32_t uptime_ms = static_cast<uint32_t>(TimerMillis());

    // Format human-readable string message
    // Note: newlib-nano (libc_nano) does not support 64-bit %llu; using %lu with 32-bit uint32_t
    // avoids corrupting subsequent variadic arguments (which previously caused cpu_temp to read 0.0C).
    snprintf(msg_text, sizeof(msg_text),
             "[Coral Micro #%lu] uptime: %lums | cpu: %.1fC",
             static_cast<unsigned long>(seq),
             static_cast<unsigned long>(uptime_ms),
             static_cast<double>(cpu_temp));

    // Serialize ROS 2 String message using Micro-CDR via picoserdes
    ros_String msg = msg_text;
    size_t cdr_len = ps_serialize(s_pub_buf, &msg, sizeof(s_pub_buf));

    if (cdr_len > 0) {
      picoros_res_t pub_res = picoros_publish(&s_publisher, s_pub_buf, cdr_len);
      if (pub_res == PICOROS_OK) {
        printf("[Pub #%lu] -> \"%s\" (%u bytes CDR)\r\n",
               static_cast<unsigned long>(seq), msg_text, static_cast<unsigned>(cdr_len));

        // Toggle User LED to indicate message sent
        user_led_state = !user_led_state;
        LedSet(Led::kUser, user_led_state);
      } else {
        printf("WARNING: Failed to publish message #%lu (res: %d)\r\n",
               static_cast<unsigned long>(seq), pub_res);
      }
    } else {
      printf("ERROR: Failed to serialize ROS 2 message #%lu\r\n", static_cast<unsigned long>(seq));
    }

    seq++;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  // Graceful cleanup
  picoros_publisher_drop(&s_publisher);
  picoros_node_drop(&s_node);
  picoros_interface_close();
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::RunPicoRosPublisher();
}
