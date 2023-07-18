#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include "Arduino.h"
#include "driver/mcpwm.h"

class MotorControl
{
public:
  MotorControl(uint16_t pin_A, uint16_t pin_B,int start_pwm = 0, int max_resolution = 512);
  
  // sets motor speed with 0 being brake
  void setSpeed(int speed);

  // sets motor speed with 0 being free rolling
  void setSpeedCoast(int speed);
  ~MotorControl();
private:
  static uint16_t _free_pwm_unit;
  static uint16_t _free_pwm_timer;
  static uint16_t _pwm_freq;
  int _start_pwm = 0;
  int _max_resolution = 512;
  uint16_t _pin_A;
  uint16_t _pin_B;
  mcpwm_unit_t _pwm_unit;
  mcpwm_timer_t _pwm_timer;
};

#endif
