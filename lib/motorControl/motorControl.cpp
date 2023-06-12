#include "motorControl.h"
uint16_t MotorControl::_free_pwm_unit=0;
uint16_t MotorControl::_free_pwm_timer=0;
uint16_t MotorControl::_pwm_freq=4000;

MotorControl::MotorControl(uint16_t pin_A, uint16_t pin_B, int start_pwm, int max_resolution) {
  _pin_A = pin_A;
  _pin_B = pin_B;
  _start_pwm = start_pwm;
  _max_resolution = max_resolution;
  if(_free_pwm_unit == 0){
    _pwm_unit = (mcpwm_unit_t)MCPWM_UNIT_0;
  }else if(_free_pwm_unit == 1){
    _pwm_unit = (mcpwm_unit_t)MCPWM_UNIT_1;
  }else{
    return;
  }
  
  // Configurazione dei pin PWM
  if (_free_pwm_timer == 0) {
    _pwm_timer = (mcpwm_timer_t)_free_pwm_timer;
    _free_pwm_timer++;
    mcpwm_gpio_init(_pwm_unit, MCPWM0A, _pin_A);
    mcpwm_gpio_init(_pwm_unit, MCPWM0B, _pin_B);
  } else
  if (_free_pwm_timer == 1) {
    _pwm_timer = (mcpwm_timer_t)_free_pwm_timer;
    _free_pwm_timer++;
    mcpwm_gpio_init(_pwm_unit, MCPWM1A, _pin_A);
    mcpwm_gpio_init(_pwm_unit, MCPWM1B, _pin_B);
  } else
  if (_free_pwm_timer == 2) {
    _pwm_timer = (mcpwm_timer_t)_free_pwm_timer;
    _free_pwm_unit++;
    _free_pwm_timer = 0;
    mcpwm_gpio_init(_pwm_unit, MCPWM2A, _pin_A);
    mcpwm_gpio_init(_pwm_unit, MCPWM2B, _pin_B);
  }else{
    return;
  }

  // Configura il PWM
  mcpwm_config_t pwm_config;
  pwm_config.frequency = _pwm_freq;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(_pwm_unit, _pwm_timer, &pwm_config);
}

MotorControl::~MotorControl(){
  if(_free_pwm_timer > 0){
    _free_pwm_timer--;
  }else if(_free_pwm_unit>0){
    _free_pwm_unit--;
  }
}

void MotorControl::setSpeed(int speed) {
  int speed_abs = abs(speed);
  int speed_sign =  (speed> 0) - (speed < 0);
  if(_start_pwm != 0){
    speed_abs = map(speed_abs,0,_max_resolution,_start_pwm,_max_resolution);
  }
  float duty = (float) speed_abs;
  duty= duty/(_max_resolution/100.0);
  if (speed < 0) {
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_A, 0);
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_B, duty);
  }
  else
  if (speed > 0) {
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_A, duty);
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_B, 0);
  }
  else
  if (speed == 0){
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_A, 100);
    mcpwm_set_duty(_pwm_unit, _pwm_timer, MCPWM_GEN_B, 100);
  }
}
