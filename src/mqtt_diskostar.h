#pragma once

#include "config.h"
#include "interface/mqtt.h"
#include "DataLed.h"

#include "ServerLed.h"
#include "motor/h_bridge_driver.h"
#include "motor/stepper_driver.h"

class MQTT_Diskostar : public MQTT
{
public:
    MQTT_Diskostar() : MQTT(MQTT_BROKER_IP, MQTT_BROKER_PORT)
    {
        // addComponent("sensor", "rotation");
        // addComponent("sensor", "position");
        // addComponent("switch", "relay");
        addComponent("light", "rotation_CW");
        addComponent("light", "rotation_CCW");
        addComponent("light", "rotation_animation_period");
        addComponent("light", "rotation_animation_amplitude");
        addComponent("light", "position");
        addComponent("light", "position_animation_period");
        addComponent("light", "position_animation_amplitude");
    }

    void setup()
    {
        if (!isRechableAndActive())
            return;

        MQTT::setup();
    }

    void setRXCallback(
        DataLed *pData,
        ServerLed &server,
        H_Bridge_Driver &motor,
        Stepper_Driver &stepper)
    {
        if (!_isActive)
            return;

        setLightChangeCallback([this,
                                pData,
                                &server,
                                &motor,
                                &stepper](
                                   const String &component_name,
                                   float percent)
                               { 
      printf("LightChangeCallback: %s, %f\n", component_name.c_str(), percent);
      bool changed = false;

      if (component_name == "rotation_CW")
      {
        pData->rotation_animation_period = 0;
        send(pData->rotation_animation_period);

        pData->rotation = util::mapConstrainf(percent, 0, 1, 0, 100);
#if ENABLE_MOTOR
        motor.setSpeed(util::centerHysteris(util::mapConstrainf(pData->rotation, -100, 100, -1.0, 1.0), 0.05));
#endif
        changed = true;
      }
      else if (component_name == "rotation_CCW")
      {
        pData->rotation_animation_period = 0;
        send(pData->rotation_animation_period);
        pData->rotation = util::mapConstrainf(percent, 0, 1, 0, -100);

#if ENABLE_MOTOR
        motor.setSpeed(util::centerHysteris(util::mapConstrainf(pData->rotation, -100, 100, -1.0, 1.0), 0.05));
#endif
        changed = true;
      }
      else if (component_name == "rotation_animation_period")
      {
        pData->rotation_animation_period = util::mapConstrainf(percent, 0, 1, 0, 100);
        changed = true;
      }
      else if (component_name == "rotation_animation_amplitude")
      {
        pData->rotation_animation_amplitude = util::mapConstrainf(percent, 0, 1, 0, 100);
        changed = true;
      }
      else if (component_name == "position")
      {
        pData->openess = util::mapConstrainf(percent, 0, 1, 0, 100);

#if ENABLE_STEPPER
        stepper.set(util::mapConstrainf(pData->openess, 0, 100, 0, pData->max_stepper_position));
#endif
        changed = true;
      }
      else if (component_name == "position_animation_period")
      {
        pData->position_animation_period = util::mapConstrainf(percent, 0, 1, 0, 100);
        changed = true;
      }
      else if (component_name == "position_animation_amplitude")
      {
        pData->position_animation_amplitude = util::mapConstrainf(percent, 0, 1, 0, 100);
        changed = true;
      }

      if (changed)
      {
        pData->save();
        server.sendAllParameters();
      } });
    }

    void sendAll()
    {
        printf("Sending ALL MQTT\n");
        send_rotation(pData);
        send(pData->openess);
        send(pData->rotation_animation_period);
        send(pData->rotation_animation_amplitude);
        send(pData->position_animation_period);
        send(pData->position_animation_amplitude);
    }

    void send(ParameterData::Parameter &param)
    {
        sendLight(param.name, param.value);
    }

    // helper bc only unipolar values for now.
    void send_rotation(DataLed *pData)
    {
        float old_rotation = NAN;

        if (old_rotation != pData->rotation.value)
        {
            old_rotation = pData->rotation.value;

            if (pData->rotation >= 0)
            {
                sendLight("rotation_CW", pData->rotation.value);
                sendLight("rotation_CCW", 0);
            }
            else
            {
                sendLight("rotation_CCW", -pData->rotation.value);
                sendLight("rotation_CW", 0);
            }
        }
    }
};