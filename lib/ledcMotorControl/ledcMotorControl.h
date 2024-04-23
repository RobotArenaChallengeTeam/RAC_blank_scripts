#ifndef ledcMotorControl_h
#define ledcMotorControl_h

#include <Arduino.h>
#include <driver/ledc.h> // Include the ESP32 LEDC library

class ledcMotorControl {
public:
    ledcMotorControl(int pin_a, int pin_b);
    void setSpeed(int speed);

private:
    int pin_a;
    int pin_b;
    int pwm_channel;
    int _max_resolution = 512;
    static int next_pwm_channel;
};

#endif
