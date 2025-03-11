#pragma once

#include "server/parameter_data.h"

// ==============================
class GenericData : public ParameterData
{
public:
#define CREATE_PARAMETER(name, defaultValue) \
    Parameter name{this, #name, int(defaultValue)};

public:
    CREATE_PARAMETER(rotation, 0.0); // -100 to 100

    CREATE_PARAMETER(actual_stepper_position, 0.0); // saved exact position for reboot retrieval
    CREATE_PARAMETER(openess, 0);                  // 0 to 100 = position
    CREATE_PARAMETER(position_offset, 0);           // -100 to 100

    CREATE_PARAMETER(stepper_max_speed, 450);
    CREATE_PARAMETER(stepper_max_acceleration, 1000);

    CREATE_PARAMETER(rotation_animation_period, 0);
    CREATE_PARAMETER(position_animation_period, 0);

    CREATE_PARAMETER(rotation_animation_amplitude, 40); // max 100
    CREATE_PARAMETER(position_animation_amplitude, 10); // max 50

    // ==============================
    // constants / not changeable
    CREATE_PARAMETER(max_stepper_position, 2250); // Range 2000-40000
    // static constexpr int max_stepper_position = 3000;
    // static constexpr int max_stepper_position = 3036;
    static constexpr int max_position_offset = 400;

    static constexpr int max_rotation_animation_period_s = 60 * 20;
    static constexpr int max_position_animation_period_s = 60 * 2;
};
