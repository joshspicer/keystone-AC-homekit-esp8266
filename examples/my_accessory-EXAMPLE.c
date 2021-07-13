/*
 * my_accessory.c
 * Define the accessory in C using the Macro in characteristics.h
 *
 *  Josh Spicer <hello@joshspicer.com>
 * 
 *  Example created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 *  
 *  
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t red_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t yellow_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t green_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t red_name = HOMEKIT_CHARACTERISTIC_(NAME, "Red");
homekit_characteristic_t yellow_name = HOMEKIT_CHARACTERISTIC_(NAME, "Yellow");
homekit_characteristic_t green_name = HOMEKIT_CHARACTERISTIC_(NAME, "Green");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_bridge, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Switch"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0000002"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        NULL
    }),
      HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
      HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
                      &red_switch_on,
                      &red_name,
                      NULL
              }),
      }),
      HOMEKIT_ACCESSORY(.id=3, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
      HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
                      &yellow_switch_on,
                      &yellow_name,
                      NULL
              }),
      }),
      HOMEKIT_ACCESSORY(.id=4, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
      HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
                      &green_switch_on,
                      &green_name,
                      NULL
              }),
      }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "588-44-001"
};
