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
const char kSsid[] = "MyWiFiSSID";
const char kPsk[] = "MyWiFiPassword";

// Zenoh Configuration - update with your Zenoh router (zenohd) IP and port
const char kRouterLocator[] = "tcp/192.168.1.100:7447";
const char kSubKeyexpr[] = "demo/coralmicro/**";

// Zenoh state
z_owned_session_t session;
z_owned_subscriber_t subscriber;
bool session_connected = false;
volatile uint32_t rx_count = 0;

void printMacAddress(uint8_t* mac) {
  for (int i = 0; i < 6; ++i) {
    if (i > 0) Serial.print(":");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
  }
}

// Callback invoked asynchronously by Zenoh-Pico background executor upon receiving a sample
void data_handler(z_loaned_sample_t* sample, void* arg) {
  rx_count++;

  z_view_string_t keystr;
  z_keyexpr_as_view_string(z_sample_keyexpr(sample), &keystr);

  z_owned_string_t payload_str;
  z_bytes_to_string(z_sample_payload(sample), &payload_str);

  Serial.print("[SUB #");
  Serial.print(rx_count);
  Serial.print("] Key: '");
  Serial.print(z_string_data(z_view_string_loan(&keystr)));
  Serial.print("' | Payload: '");
  Serial.print(z_string_data(z_string_loan(&payload_str)));
  Serial.println("'");

  // Toggle User LED on message reception
  digitalWrite(PIN_LED_USER, (rx_count % 2 == 0) ? HIGH : LOW);

  z_string_drop(z_string_move(&payload_str));
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }

  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_LED_USER, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  digitalWrite(PIN_LED_USER, LOW);

  Serial.println();
  Serial.println("==================================================");
  Serial.println("   Google Coral Dev Board Micro - Zenoh Subscriber");
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

  // 4. Declare Zenoh subscriber
  z_owned_closure_sample_t callback;
  z_closure_sample(&callback, data_handler, nullptr, nullptr);

  z_view_keyexpr_t ke;
  z_view_keyexpr_from_str_unchecked(&ke, kSubKeyexpr);

  if (z_declare_subscriber(z_loan(session), &subscriber, z_loan(ke), z_move(callback), nullptr) < 0) {
    Serial.println("[ERROR] Failed to declare Zenoh subscriber!");
    z_drop(z_move(session));
    return;
  }
  Serial.print("Zenoh subscriber declared on: ");
  Serial.println(kSubKeyexpr);
  Serial.println("Waiting for incoming messages...");
  Serial.println("--------------------------------------------------");

  session_connected = true;
}

void loop() {
  if (!session_connected) {
    digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
    delay(1000);
    return;
  }

  // Zenoh samples are processed in background FreeRTOS executor task
  delay(500);
}
