#pragma once

// esp32-42C2E4
// D8:13:2A:42:C2:E4

#include "util.h"

#define DEVICE_NAME "disko-star"

// ================== FEATURES ====================
#define ENABLE_SERIAL 1

#define ENABLE_SERVER 1
#define ENABLE_MQTT 0

#define ENABLE_MOTOR 1
#define ENABLE_STEPPER 1

#define ENABLE_LEDS 0


// ================== Settings ====================

#if ENABLE_MQTT

#if 1
#define MQTT_BROKER_IP "192.168.178.41" // @ Home
#else
#define MQTT_BROKER_IP "192.168.0.144" // mobile router:
#endif

#define MQTT_BROKER_PORT 1883

#endif

// ================ Development ===================

#define SERIAL_BAUD_RATE 115200
