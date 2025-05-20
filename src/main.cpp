#include <Arduino.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "pinout.h"

#include "util.h"

#if ENABLE_LED_CHAIN
#include "led_matrix.h"
#endif

#if ENABLE_SERVER
#include "ServerLed.h"
extern ServerLed server;
ServerDataLed*   pData = nullptr;
#endif

#if ENABLE_MQTT
#include "MQTTLed.h"
MQTTLed mqtt;
#endif

#if ENABLE_MOTOR
#include "motor/h_bridge_driver.h"
H_Bridge_Driver motor(PIN_MOTOR1_IN1, PIN_MOTOR1_IN2);
#endif

#if ENABLE_LED_CHAIN
LedMatrix matrix(PIN_LED_MATRIX, 8 * 4, 8); // PIN=5, width=32, height=8
#endif



void setup()
{
#if ENABLE_LED_CHAIN
    matrix.init();
    matrix.showText("DISKO!", matrix.Color(255, 255, 255));
#endif

#if ENABLE_MOTOR
    motor.setup();
#endif


#if ENABLE_SERIAL
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000); // give Serial Monitor time to connect
#endif

#if ENABLE_SERVER
    server.setup();
    pData = &server.data;
#endif

#if ENABLE_MQTT
    mqtt.setup();
    mqtt.setRXCallback(pData, server);
#endif

    ArduinoOTA.begin();

    delay(1000);

    printf("setup() done!\n");
}

elapsedMillis _timer_debug          = 0;
elapsedMillis _sinceAnimationUpdate = 0;

void loop()
{
#if ENABLE_LED_CHAIN
    matrix.loop();
#endif

    ArduinoOTA.handle();

#if ENABLE_SERVER
    server.loop();

#if ENABLE_MQTT
    mqtt.loop();
#endif

    if (pData->wasUpdated())
    {

#if ENABLE_MOTOR
        if (pData->rotation_animation_period == 0)
            motor.setSpeed(
                util::centerHysteris(util::mapConstrainf(pData->rotation, -100, 100, -1.0, 1.0),
                                     0.05));
#endif

#if ENABLE_MQTT
        mqtt.sendAll();
#endif
    }
#endif

    if (_sinceAnimationUpdate > 200)
    {
        _sinceAnimationUpdate = 0;
    }

    if (_timer_debug > 50)
    {
        _timer_debug = 0;
    }
}
