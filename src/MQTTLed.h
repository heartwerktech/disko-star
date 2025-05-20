#pragma once

#include "config.h"
#include "interface/mqtt.h"

#include "ServerDataLed.h"
#if ENABLE_SERVER
#include "ServerLed.h"
#else
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#endif

#include "LedDriver.h"

class MQTTLed : public MQTT
{
public:
    MQTTLed()
        : MQTT(MQTT_BROKER_IP, MQTT_BROKER_PORT)
    {
        // addComponent("sensor", "rotation");
        addComponent("light", "driver_ch1");
        addComponent("light", "driver_ch2");
        addComponent("light", "driver_ch3");
        addComponent("light", "driver_ch4");
    }

    void setup()
    {
        if (!isRechableAndActive())
        {
            printf("MQTT not reachable at: %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            return;
        }
        else
            printf("Connecting to MQTT Server\n");

        MQTT::setup();
    }

#if ENABLE_SERVER

    void setRXCallback(ServerDataLed* pData, ServerLed& server)
    {
        if (!_isActive)
            return;

        setLightChangeCallback([this, pData, &server](const String& component_name, float percent) {
            printf("LightChangeCallback: %s, %f\n", component_name.c_str(), percent);
            bool changed = false;

            // check if component_name contains "driver_ch"
            if (component_name.startsWith("driver_ch"))
            {
                int ch = component_name.substring(9).toInt();
                switch (ch)
                {
                case 1:
                    pData->driver_ch1.value = percent;
                    break;
                case 2:
                    pData->driver_ch2.value = percent;
                    break;
                case 3:
                    pData->driver_ch3.value = percent;
                    break;
                case 4:
                    pData->driver_ch4.value = percent;
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
#endif

    void setRXCallback(LedDrivers& led_drivers)
    {
        if (!_isActive)
            return;

        setLightChangeCallback([this, &led_drivers](const String& component_name, float percent) {
            printf("LightChangeCallback: %s, %f\n", component_name.c_str(), percent);

            // check if component_name contains "driver_ch"
            if (component_name.startsWith("driver_ch"))
            {
                int ch = component_name.substring(9).toInt() - 1;

                printf("led_drivers[%d].set(%2.2f)\n", ch, percent);
                led_drivers[ch].set(percent);

                sendLightPercent("driver_ch" + String(ch + 1), percent);
            }
        });

        setLightToggleCallback([this, &led_drivers](const String& component_name, bool state) {
            printf("LightToggleCallback: %s, %d\n", component_name.c_str(), state);
            if (component_name.startsWith("driver_ch"))
            {
                int ch = component_name.substring(9).toInt() - 1;
                led_drivers[ch].toggle(state);
                sendLightPercent(component_name, led_drivers[ch].get());
            }
        });

        for (unsigned int i = 0; i < led_drivers.size(); i++)
            led_drivers[i].setOnSelfUpdate([this, &led_drivers, i]() {
                printf("Self Updated led_drivers[%d] to %2.2f\n", i, led_drivers[i].get());
                for (unsigned int i = 0; i < led_drivers.size(); i++)
                    sendLightPercent("driver_ch" + String(i + 1), led_drivers[i].get());
            });
    }

#if ENABLE_SERVER
    void sendAll()
    {
        printf("Sending ALL MQTT\n");

        for (unsigned int i = 0; i < led_drivers.size(); i++)
            sendLightPercent("driver_ch" + String(i + 1), led_drivers[i].get());
    }
#endif

    void send(ParameterData::Parameter& param) { sendLight(param.name, param.value); }
};