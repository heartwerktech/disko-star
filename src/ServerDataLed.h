#pragma once

#include <vector>

#include "server/parameter_data.h"

// ==============================
class ServerDataLed : public ParameterData
{
public:

public:
    CREATE_PARAMETER(driver_ch1, 0.0); // 0 to 100
    CREATE_PARAMETER(driver_ch2, 0.0); // 0 to 100
    CREATE_PARAMETER(driver_ch3, 0.0); // 0 to 100
    CREATE_PARAMETER(driver_ch4, 0.0); // 0 to 100

    // std vector of all led Parameter
    std::vector<Parameter *> driver_channels = {
        &driver_ch1,
        &driver_ch2,
        &driver_ch3,
        &driver_ch4
    };

    // static constexpr int someconstant = 60 * 2;
};
