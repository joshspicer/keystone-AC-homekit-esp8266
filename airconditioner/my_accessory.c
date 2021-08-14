
/*
*
*
*
*/


#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// Heater/Cooler - (HAP 8.18)

// -- Required Characteristics --
// ”9.3 Active” (page 159) 
// ”9.35 Current Temperature” (page 175)
// ”9.33 Current Heater Cooler State” (page 174) - 
// ”9.111 Target Heater Cooler State” (page 223) - 

// -- Optional Characteristics --
// ”9.62 Name” (page 188)
// ”9.81 Rotation Speed” (page 197) **
// ”9.122 Temperature Display Units” (page 231)
// ”9.107 Swing Mode” (page 221)
// ”9.20 Cooling Threshold Temperature” (page 167)  **
// ”9.42 Heating Threshold Temperature” (page 178)
// ”9.55 Lock Physical Controls” (page 185)

// 0:Inactive, 1:Active
homekit_characteristic_t cooler_active = HOMEKIT_CHARACTERISTIC_(ACTIVE, 0);

// float; min 0, max 100, step 0.1, unit celsius
homekit_characteristic_t current_temp = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);

// 0:Off, 1:Idle,  2:Heating, 3:Cooling
homekit_characteristic_t current_state = HOMEKIT_CHARACTERISTIC_(CURRENT_HEATER_COOLER_STATE, 0);

// float: 10-35
homekit_characteristic_t target_state = HOMEKIT_CHARACTERISTIC_(TARGET_HEATER_COOLER_STATE, 0);

// float 0-100
homekit_characteristic_t rotation_speed = HOMEKIT_CHARACTERISTIC_(ROTATION_SPEED, 0);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_air_conditioner, .services=(homekit_service_t*[]) {
         HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
                HOMEKIT_CHARACTERISTIC(NAME, "Air Conditioner"),
                HOMEKIT_CHARACTERISTIC(MANUFACTURER, "spicer.dev"),
                HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0000001"),
                HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
                HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
                HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                NULL
         }),
       HOMEKIT_SERVICE(HEATER_COOLER, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
           &cooler_active,
           &current_temp,
           &current_state,
           &target_state,
           &rotation_speed,
           NULL
       }),
        NULL
    }),
      NULL
};


homekit_server_config_t config = {
    .accessories = accessories,
    .password = "134-11-134"
};
