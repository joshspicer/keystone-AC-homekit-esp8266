
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

/*
 *  Modified on: 2021-07-13
 *  Josh Spicer <hello@joshspicer.com>
 *      
 *  Built with the following libraries:
 *     https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266 
 *     https://github.com/crankyoldgit/IRremoteESP8266
 *
 */

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
  ac.begin();
}

void loop() {
  my_homekit_loop();
  delay(10);
  ac.send();
}

// defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cooler_active;
extern "C" homekit_characteristic_t current_temp;
extern "C" homekit_characteristic_t current_state;
extern "C" homekit_characteristic_t target_state;

extern "C" homekit_characteristic_t fan_active;
extern "C" homekit_characteristic_t fan_speed;

static uint32_t next_heap_millis = 0;

void cooler_active_setter(const homekit_value_t value) {
  bool state = value.bool_value;
  cooler_active.value.bool_value = state;  //sync the value
  LOG_D("COOLER_ACTIVE: %s", state ? "ON" : "OFF");

  if (state) {
      ac.on();
  } else {
      ac.off();
  }
}

// IR settings
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRMideaAC ac(kIrLed);  // Set the GPIO to be used to sending the message

void my_homekit_setup() {
  //Add the .setter function to get the switch-event sent from iOS Home APP.
  //The .setter should be added before arduino_homekit_setup.
  //HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
  //Maybe this is a legacy design issue in the original esp-homekit library,
  //and I have no reason to modify this "feature".
  cooler_active.setter = cooler_active_setter;
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
