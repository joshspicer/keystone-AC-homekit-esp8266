/*
 *  Modified on: 2021-07-13
 *  Josh Spicer <hello@joshspicer.com>
 *      
 *  Built with the following libraries:
 *     https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266 
 *     https://github.com/crankyoldgit/IRremoteESP8266
 *
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t red_switch_on;
extern "C" homekit_characteristic_t yellow_switch_on;
extern "C" homekit_characteristic_t green_switch_on;


static uint32_t next_heap_millis = 0;

#define LED_RED 1
#define LED_YELLOW 3
#define LED_GREEN 15

void red_switch_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  red_switch_on.value.bool_value = on;  //sync the value
  LOG_D("RED: %s", on ? "ON" : "OFF");
  digitalWrite(LED_RED, on ? HIGH : LOW);
}

void yellow_switch_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  yellow_switch_on.value.bool_value = on;  //sync the value
  LOG_D("YELLOW: %s", on ? "ON" : "OFF");
  digitalWrite(LED_YELLOW, on ? HIGH : LOW);
}

void green_switch_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  green_switch_on.value.bool_value = on;  //sync the value
  LOG_D("GREEN: %s", on ? "ON" : "OFF");
  digitalWrite(LED_GREEN, on ? HIGH : LOW);
}


void my_homekit_setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  

  //Add the .setter function to get the switch-event sent from iOS Home APP.
  //The .setter should be added before arduino_homekit_setup.
  //HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
  //Maybe this is a legacy design issue in the original esp-homekit library,
  //and I have no reason to modify this "feature".
  red_switch_on.setter = red_switch_on_setter;
  yellow_switch_on.setter = yellow_switch_on_setter;
  green_switch_on.setter = green_switch_on_setter;
  arduino_homekit_setup(&config);

  //report the switch value to HomeKit if it is changed (e.g. by a physical button)
  //bool switch_is_on = true/false;
  //cha_switch_on.value.bool_value = switch_is_on;
  //homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}
