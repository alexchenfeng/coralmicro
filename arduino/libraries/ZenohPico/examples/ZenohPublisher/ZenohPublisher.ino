/*
 * Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ZenohPico.h>

// Wi-Fi Configuration - update with your access point credentials
const char kSsid[] = "Matrix2.4G";
const char kPsk[] = "m@BgLn8U7iCN";

// Zenoh Configuration - update with your Zenoh router (zenohd) IP and port
const char kRouterLocator[] = "tcp/192.168.1.159:7447";
const char kTopicKeyexpr[] = "demo/coralmicro/telemetry";

// Zenoh state
z_owned_session_t session;
z_owned_publisher_t pub;
bool session_connected = false;
uint32_t seq = 0;

void printMacAddress(uint8_t* mac) {
  for (int i = 0; i < 6; ++i) {
    if (i > 0) Serial.print(":");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    // Wait for serial monitor connection (up to 3 seconds)
    delay(10);
  }

  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_LED_USER, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  digitalWrite(PIN_LED_USER, LOW);

  Serial.println();
  Serial.println("==================================================");
  Serial.println("   Google Coral Dev Board Micro - Zenoh Publisher ");
  Serial.println("==================================================");

  // 1. Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(kSsid);

  int wifi_status = (strlen(kPsk) > 0) ? WiFi.begin(kSsid, kPsk) : WiFi.begin(kSsid);
  if (wifi_status != WL_CONNECTED) {
    Serial.println("[ERROR] Failed to connect to Wi-Fi network!");
    return;
  }

  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("Wi-Fi connected! Board MAC: ");
  printMacAddress(mac);
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());

  // 2. Configure Zenoh client session
  Serial.println("Configuring Zenoh-Pico client session...");
  z_owned_config_t config;
  z_config_default(&config);
  zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, "client");
  zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, kRouterLocator);

  // 3. Open Zenoh session
  Serial.print("Connecting to Zenoh router at: ");
  Serial.println(kRouterLocator);

  int8_t open_res = z_open(&session, z_move(config), nullptr);
  if (open_res < 0) {
    Serial.print("[ERROR] Failed to open Zenoh session! Error code: ");
    Serial.println((int)open_res);
    Serial.println("Please check router IP/port and ensure 'zenohd' is running.");
    return;
  }
  Serial.println("Zenoh session established successfully!");

  // 4. Declare Zenoh publisher
  z_view_keyexpr_t ke;
  z_view_keyexpr_from_str_unchecked(&ke, kTopicKeyexpr);
  if (z_declare_publisher(z_loan(session), &pub, z_loan(ke), nullptr) < 0) {
    Serial.println("[ERROR] Failed to declare Zenoh publisher!");
    z_drop(z_move(session));
    return;
  }
  Serial.print("Zenoh publisher declared on topic: ");
  Serial.println(kTopicKeyexpr);
  Serial.println("--------------------------------------------------");

  session_connected = true;
}

void loop() {
  if (!session_connected) {
    // Session not active; blink status LED slowly to indicate error state
    digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
    delay(1000);
    return;
  }

  // Format telemetry JSON payload
  char payload_buf[160];
  unsigned long uptime_ms = millis();
  snprintf(payload_buf, sizeof(payload_buf),
           "{\"seq\":%lu,\"uptime_ms\":%lu,\"device\":\"coral_micro_wifi\"}",
           (unsigned long)seq, uptime_ms);

  z_owned_bytes_t payload;
  z_bytes_copy_from_str(&payload, payload_buf);

  z_publisher_put_options_t options;
  z_publisher_put_options_default(&options);

  // Publish telemetry payload to Zenoh router
  z_result_t res = z_publisher_put(z_loan(pub), z_move(payload), &options);
  if (res >= 0) {
    Serial.print("[PUB #");
    Serial.print(seq);
    Serial.print("] (");
    Serial.print(strlen(payload_buf));
    Serial.print(" bytes) -> ");
    Serial.println(payload_buf);

    // Toggle User LED on successful publish
    digitalWrite(PIN_LED_USER, (seq % 2 == 0) ? HIGH : LOW);
  } else {
    Serial.print("[WARN] Failed to publish message #");
    Serial.println(seq);
  }

  seq++;
  delay(1000);
}
