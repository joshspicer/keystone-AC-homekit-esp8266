/*
*
*
*
*/

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// Heater/Cooler - (HAP 8.18)

// -- Required Characteristics --
// ”9.3 Active” (page 159)
// ”9.35 Current Temperature” (page 175)
// ”9.33 Current Heater Cooler State” (page 174)
// ”9.111 Target Heater Cooler State” (page 223)

// -- Optional Characteristics --
// ”9.62 Name” (page 188)
// ”9.81 Rotation Speed” (page 197)
// ”9.122 Temperature Display Units” (page 231)
// ”9.107 Swing Mode” (page 221)
// ”9.20 Cooling Threshold Temperature” (page 167)
// ”9.42 Heating Threshold Temperature” (page 178)
// ”9.55 Lock Physical Controls” (

homekit_characteristic_t active = HOMEKIT_CHARACTERISTIC_(ACTIVE, 0);
homekit_characteristic_t current_temp = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
