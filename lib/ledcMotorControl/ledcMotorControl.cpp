#include "ledcMotorControl.h"

int ledcMotorControl::next_pwm_channel = LEDC_CHANNEL_0;

ledcMotorControl::ledcMotorControl(int pin_a, int pin_b) : pin_a(pin_a), pin_b(pin_b) {
    pinMode(pin_a, OUTPUT); // Configure pin_a as output
    pinMode(pin_b, OUTPUT); // Configure pin_b as output

    pwm_channel = next_pwm_channel;
    next_pwm_channel = next_pwm_channel+2;
    if (next_pwm_channel > LEDC_CHANNEL_MAX) {
        next_pwm_channel = LEDC_CHANNEL_0;
    }

    // Set up LEDC for PWM with 9-bit resolution
    ledcSetup(pwm_channel, 1500, LEDC_TIMER_9_BIT); // 1500 Hz PWM frequency, 9-bit resolution
    ledcSetup(pwm_channel + 1, 1500, LEDC_TIMER_9_BIT);

    // Attach pins to the LEDC channels
    ledcAttachPin(pin_a, pwm_channel);
    ledcAttachPin(pin_b, pwm_channel + 1);
}

void ledcMotorControl::setSpeed(int speed) {
    speed = constrain(speed,-_max_resolution,_max_resolution-1);
    if (speed > 0) {
        ledcWrite(pwm_channel, speed);
        ledcWrite(pwm_channel + 1, 0);
    } else if (speed < 0) {
        ledcWrite(pwm_channel + 1, -speed);
        ledcWrite(pwm_channel, 0);
    } else {
        ledcWrite(pwm_channel, _max_resolution-1);
        ledcWrite(pwm_channel + 1, _max_resolution-1);
    }
}
