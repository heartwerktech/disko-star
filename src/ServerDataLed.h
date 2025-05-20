#pragma once

#include <vector>

#include "server/parameter_data.h"

// ==============================
class ServerDataLed : public ParameterData
{
public:

public:
    CREATE_PARAMETER(led_ch1, 0.0); // 0 to 100
    CREATE_PARAMETER(led_ch2, 0.0); // 0 to 100
    CREATE_PARAMETER(led_ch3, 0.0); // 0 to 100
    CREATE_PARAMETER(led_ch4, 0.0); // 0 to 100

    // std vector of all led Parameter
    std::vector<Parameter *> led_channels = {
        &led_ch1,
        &led_ch2,
        &led_ch3,
        &led_ch4
    };

    // static constexpr int someconstant = 60 * 2;
};
