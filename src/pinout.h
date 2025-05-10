#pragma once

#if 0 // hannes
#define PIN_STEPPER_IN1 27
#define PIN_STEPPER_IN2 25
#define PIN_STEPPER_IN3 32
#define PIN_STEPPER_IN4 12

#elif 1 // fablab

#define PIN_STEPPER_IN1 5 // pin5 	outputs PWM signal at boot, strapping pin dont use in future
#define PIN_STEPPER_IN2 23
#define PIN_STEPPER_IN3 19
#define PIN_STEPPER_IN4 18

#endif

#if 0 // hannes
#define PIN_MOTOR1_IN1 14
#define PIN_MOTOR1_IN2 33
#define PIN_MOTOR2_IN1 34 // not used
#define PIN_MOTOR2_IN2 35 
#else // fablab
#define PIN_MOTOR1_IN1 27
#define PIN_MOTOR1_IN2 25
#define PIN_MOTOR2_IN1 32 // not used
#define PIN_MOTOR2_IN2 12 // not used

#endif

#define PIN_LED_MATRIX 5 // pin5 	outputs PWM signal at boot, strapping pin dont use in future

