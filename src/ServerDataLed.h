#pragma once

#include "server/parameter_data.h"

// ==============================
class ServerDataLed : public ParameterData
{
public:

public:
    CREATE_PARAMETER(light_ch1, 0.0); // 0 to 100
    CREATE_PARAMETER(light_ch2, 0.0); // 0 to 100
    CREATE_PARAMETER(light_ch3, 0.0); // 0 to 100
    CREATE_PARAMETER(light_ch4, 0.0); // 0 to 100

    // static constexpr int someconstant = 60 * 2;
};
