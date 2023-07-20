#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include "Arduino.h"
#include "driver/mcpwm.h"

/**
 * @brief motor control object to control DRV8833 or any pwm-pwm driver with esp mcpwm
 * @param pin_A drv8833 input pin
 * @param pin_B drv8833 input pin
 * @param start_speed minimum speed given to the motor when using setspeed(spd) with spd != 0,
 * useful to remove low pwm whistling, default 100
 * @param pwm_freq hz freqency for the pwm driver on this motor, default 980hz
 * @param max_speed speed number that will result in 100% pwm duty cycle, default 512
 * 
 * @note if the motor is turining in the wrong direction reverse pin_a and pin_b
 */
class MotorControl
{
public:
  MotorControl(uint16_t pin_A, uint16_t pin_B,int start_speed = 100,int pwm_freq = 980,int max_resolution = 512);
  
  /**
 * @brief set the motor speed with 0 being motor brake, grants faster stopping
 * @param speed motor speed ranging from -max_speed (reverse) to +max_speed (forward)
 */
  void setSpeed(int speed);
  /**
 * @brief set the motor speed with 0 being motor free rolling, allows easier backdrivability
 * @param speed motor speed ranging from -max_speed (reverse) to +max_speed (forward)
 */
  void setSpeedCoast(int speed);

  ~MotorControl();
private:
  static uint16_t _free_pwm_unit;
  static uint16_t _free_pwm_timer;
  int _start_speed = 0;
  int _max_resolution = 512;
  uint16_t _pin_A;
  uint16_t _pin_B;
  mcpwm_unit_t _pwm_unit;
  mcpwm_timer_t _pwm_timer;
};

#endif
