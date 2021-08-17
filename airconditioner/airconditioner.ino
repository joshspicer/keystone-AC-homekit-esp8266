
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

/*
 *  Modified on: 2021-08-10
 *  Josh Spicer <hello@joshspicer.com>
 *  Writeup: http://spcr.me/aircon-homekit
 *      
 *  Built with the following libraries:
 *     https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266 
 *     https://github.com/crankyoldgit/IRremoteESP8266
 *
 */

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// IR settings
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRMideaAC ac(kIrLed);  // Set the GPIO to be used to sending the message

// Globals
bool queueCommand = false;
void flipQueueCommand(bool newState) {
  Serial.write("Flipping queueCommand to %d\n", newState);
  queueCommand = newState;
}

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
  Serial.write("HomeKit setup complete. About to start ac.begin()\n");
  ac.begin();
  ac.setUseCelsius(false);
}

void loop() {
  my_homekit_loop();
  delay(10);

  if (queueCommand)
  {
    Serial.write("Sending AC Command....\n");
    ac.send();
    flipQueueCommand(false);
  }
  
}

// defined in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cooler_active;
extern "C" homekit_characteristic_t current_temp;
extern "C" homekit_characteristic_t current_state;
extern "C" homekit_characteristic_t target_state;
extern "C" homekit_characteristic_t rotation_speed;
extern "C" homekit_characteristic_t cooling_threshold;

static uint32_t next_heap_millis = 0;

// --- Setters
void cooler_active_setter(const homekit_value_t value) {
  bool oldState = cooler_active.value.bool_value;
  bool state = value.bool_value;
  cooler_active.value = value;  //sync the value
  
  LOG_D("ACTIVE was %s and is now set to %s", oldState ? "ON" : "OFF", state ? "ON" : "OFF");
  
  if (state) {
      ac.on();
  } else {
      ac.off();
  }

  flipQueueCommand(true);
}

void current_temp_setter(const homekit_value_t value) {
  LOG_D("NO_OP: current_temp_setter Got value %d", value.float_value);
}

void current_state_setter(const homekit_value_t value) {
  LOG_D("NO_OP: current_state_setter. Got value %d", value.int_value);
}

void target_state_setter(const homekit_value_t value) {
  int oldState = target_state.value.int_value;
  int newState = value.int_value;
  
  target_state.value = value; //sync value
  current_state.value = value; // sync value
  LOG_D("target_state_setter was %d and just got value %d", oldState, newState);

  if (oldState == newState) {
    LOG_D("State setter invoked but did not change. Skipping.");
    return;
  }

  // 0: auto, 1: heat, 2:cool
  switch(newState) {
    case 0:
      ac.setMode(kMideaACAuto);
      break;
    case 1:
      LOG_D("heating not supported");
      return;
    case 2:
      ac.setMode(kMideaACCool);
      break;
  }

  flipQueueCommand(true);
}

void rotation_speed_setter(const homekit_value_t value) {
  float oldSpeed = rotation_speed.value.float_value;
  float newSpeed = value.float_value;
  rotation_speed.value = value;  //sync the value

  LOG_D("ROTATION SPEED was %.2f and is now set to %.2f", oldSpeed, newSpeed);

  if (oldSpeed == newSpeed) {
    LOG_D("Rotation speed NO-OP");
    return;
  }

  int fanSpeed = 0; // fan mode disabled
  if (newSpeed < 33){
    fanSpeed = 1;
  } else if (newSpeed < 66) {
    fanSpeed = 2;
  } else {
    fanSpeed = 3;
  }

  
  // Enable just fan mode if fanSpeed > 0
  if (fanSpeed == 0) {
    ac.setMode(kMideaACAuto);
  } else {
    LOG_D("Enabling fan mode");
    ac.setMode(kMideaACFan);
  }
  
   LOG_D("ROTATION speed ac value is %d", fanSpeed);
   ac.setFan(fanSpeed);
   flipQueueCommand(true);
}

void cooling_threshold_setter(const homekit_value_t value) {
  float oldTemp = cooling_threshold.value.float_value;
  float temp = value.float_value;
  cooling_threshold.value = value;  //sync the value

  LOG_D("COOLER THRESHOLD was %.2f and is now set to %.2f", oldTemp, temp);

  ac.setTemp(temp, true);
  flipQueueCommand(true);
}


// --- End Setters

void my_homekit_setup() {
  Serial.write("starting my_homekit_setup\n");
  //Add the .setter function to get the switch-event sent from iOS Home APP.
  //The .setter should be added before arduino_homekit_setup.
  //HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
  //Maybe this is a legacy design issue in the original esp-homekit library,
  //and I have no reason to modify this "feature".
  
  cooler_active.setter = cooler_active_setter;
  current_temp.setter = current_temp_setter;
  current_state.setter = current_state_setter;
  target_state.setter = target_state_setter;
  rotation_speed.setter = rotation_speed_setter;
  cooling_threshold.setter = cooling_threshold_setter;

  Serial.write("about to call arduino_homekit_setup\n");
  arduino_homekit_setup(&config);

  //report the switch value HERE to HomeKit if it is changed (e.g. by a physical button)

  Serial.write("exiting my_homekit_setup\n");
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
