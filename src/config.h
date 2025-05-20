#pragma once

// esp32-42C2E4
// D8:13:2A:42:C2:E4

#include "util.h"

#define DEVICE_NAME "led-driver"

// ================== FEATURES ====================
#define ENABLE_SERIAL 1

#define ENABLE_SERVER 0
#define ENABLE_MQTT 1

#define ENABLE_MOTOR 0
#define ENABLE_LED_CHAIN 0
#define ENABLE_LED_DRIVER 1

// ================== Settings ====================

#if ENABLE_MQTT

#if 1
#define MQTT_BROKER_IP "192.168.178.41" // @ Home
#else
#define MQTT_BROKER_IP "192.168.0.144" // mobile router:
#endif

// steps to change to mobile router
// - change homeassistant mqtt broker in integration to above
// - recompile disko-star repo to use different ip from above
// - reconfigure wled to use correct ip

#define MQTT_BROKER_PORT 1883

#endif

// ================ Development ===================

#define SERIAL_BAUD_RATE 115200
