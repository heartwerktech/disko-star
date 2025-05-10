#include <Arduino.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "pinout.h"

#include "util.h"

#if ENABLE_LEDS
#include "led_matrix.h"
#endif

#if ENABLE_SERVER
#include "generic_server.h"
extern GenericServer server;
GenericData *pData = nullptr;
#endif

#if ENABLE_MQTT
#include "mqtt_diskostar.h"
MQTT_Diskostar mqtt;
#endif

#if ENABLE_MOTOR
#include "motor/h_bridge_driver.h"
H_Bridge_Driver motor(PIN_MOTOR1_IN1, PIN_MOTOR1_IN2);
#endif

#if ENABLE_STEPPER
#include "motor/stepper_driver.h"
Stepper_Driver stepper(PIN_STEPPER_IN1, PIN_STEPPER_IN2, PIN_STEPPER_IN3, PIN_STEPPER_IN4);
#endif

#if ENABLE_LEDS
LedMatrix matrix(PIN_LED_MATRIX, 8 * 4, 8); // PIN=5, width=32, height=8
#endif

void setup()  
{
#if ENABLE_LEDS
  matrix.init();
  matrix.showText("DISKO!", matrix.Color(255, 255, 255));
#endif

#if ENABLE_MOTOR
  motor.setup();
#endif

#if ENABLE_STEPPER
  stepper.setup();
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
  mqtt.setRXCallback(pData, server, motor, stepper);
#endif

  ArduinoOTA.begin();

#if ENABLE_STEPPER
  stepper.setMaxSpeed(pData->stepper_max_speed);
  stepper.setAcceleration(pData->stepper_max_acceleration);
  stepper.setCurrentPosition(pData->actual_stepper_position);
#endif

  delay(1000);

  printf("Setup done!\n");
}

elapsedMillis _timer_debug = 0;
elapsedMillis _timeElapsed = 0;
elapsedMillis _sinceAnimationUpdate = 0;

void loop()
{
#if ENABLE_LEDS
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
      motor.setDirectly(util::centerHysteris(util::mapConstrainf(pData->rotation, -100, 100, -1.0, 1.0), 0.05));
#endif

#if ENABLE_STEPPER
    stepper.setMaxSpeed(pData->stepper_max_speed);
    stepper.setAcceleration(pData->stepper_max_acceleration);

    if (pData->position_animation_period == 0)
    {
      stepper.set(util::mapConstrainf(pData->openess, 0, 100, 0, pData->max_stepper_position));
      if (pData->position_offset != 0)
      {
        int offset_actual = util::mapConstrainf(pData->position_offset, -100, 100, -pData->max_position_offset, pData->max_position_offset);
        stepper.setOffset(offset_actual);
      }
    }
#endif

#if ENABLE_MQTT
    mqtt.sendAll();
#endif
  }
#endif

  if (_sinceAnimationUpdate > 200)
  {
    _sinceAnimationUpdate = 0;
    if (pData->rotation_animation_period != 0)
    {
      float period_seconds = util::mapConstrainf(pData->rotation_animation_period, 0, 100, 1, pData->max_rotation_animation_period_s);
      float freq = 1.0 / period_seconds;
      float val = sin(_timeElapsed / 1000.0 * 2 * PI * freq);
      pData->rotation = val * pData->rotation_animation_amplitude;
#if ENABLE_MQTT
      mqtt.send_rotation(pData);
#endif

#if ENABLE_MOTOR
      motor.setSpeed(util::centerHysteris(util::mapConstrainf(pData->rotation, -100, 100, -1.0, 1.0), 0.05));
#endif
    }

    if (pData->position_animation_period != 0)
    {
      float period_seconds = util::mapConstrainf(pData->position_animation_period, 0, 100, 1, pData->max_position_animation_period_s);
      float freq = 1.0 / period_seconds;
      float val = 0.5 + 0.5 * sin(_timeElapsed / 1000.0 * 2 * PI * freq);
      pData->openess = 100 - val * pData->position_animation_amplitude;
#if ENABLE_MQTT
      mqtt.send(pData->openess);
#endif

#if ENABLE_STEPPER
      stepper.set(util::mapConstrainf(pData->openess, 0, 100, 0, pData->max_stepper_position));
#endif
    }
  }

#if ENABLE_STEPPER
  if (stepper.reachedGoal())
  {
    if (pData->position_offset != 0)
    {
      int offset_actual = util::mapConstrainf(pData->position_offset, -100, 100, -pData->max_position_offset, pData->max_position_offset);
      stepper.setCurrentPosition(stepper.currentPosition() - offset_actual);
      pData->position_offset = 0;
      server.sendAllParameters();
    }

    pData->actual_stepper_position = stepper.currentPosition();
    pData->save();
    printf("Reached + Saved Position: %d\n", pData->actual_stepper_position.value);
  }
#endif

  if (_timer_debug > 50)
  {
    _timer_debug = 0;
  }

#if ENABLE_MOTOR
  motor.loop();
#endif

#if ENABLE_STEPPER
  stepper.loop();
#endif
}
