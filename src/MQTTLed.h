#pragma once

#include "config.h"
#include "interface/mqtt.h"

#include "ServerDataLed.h"
#include "ServerLed.h"

#include "motor/h_bridge_driver.h"

class MQTTLed : public MQTT
{
public:
    MQTTLed()
        : MQTT(MQTT_BROKER_IP, MQTT_BROKER_PORT)
    {
        // addComponent("sensor", "rotation");
        addComponent("light", "light_ch1");
        addComponent("light", "light_ch2");
        addComponent("light", "light_ch3");
        addComponent("light", "light_ch4");
    }

    void setup()
    {
        if (!isRechableAndActive())
            return;

        MQTT::setup();
    }

    void setRXCallback(ServerDataLed*   pData,
                       ServerLed&       server)
    {
        if (!_isActive)
            return;

        setLightChangeCallback([this, pData, &server](const String& component_name, float percent) {
            printf("LightChangeCallback: %s, %f\n", component_name.c_str(), percent);
            bool changed = false;

            // check if component_name contains "light_ch"
            if (component_name.startsWith("light_ch"))
            {
                int ch = component_name.substring(9).toInt();
                switch (ch)
                {
                    case 1:
                        pData->light_ch1.value = percent;
                        break;
                    case 2:
                        pData->light_ch2.value = percent;
                        break;
                    case 3:
                        pData->light_ch3.value = percent;
                        break;
                    case 4:
                        pData->light_ch4.value = percent;
                        break;
                }
                changed = true;
            }

            if (changed)
            {
                pData->save();
                server.sendAllParameters();
            }
        });
    }

    void sendAll()
    {
        printf("Sending ALL MQTT\n");
        send(pData->light_ch1);
        send(pData->light_ch2);
        send(pData->light_ch3);
        send(pData->light_ch4);
    }

    void send(ParameterData::Parameter& param) { sendLight(param.name, param.value); }

};