#include "ledUtility.h"
#include "esp_log.h"
uint8_t LedUtility::_pin;
bool LedUtility::_led_state;
int16_t LedUtility::_led_i;
int16_t LedUtility::_blinks;
uint16_t LedUtility::_blink_delay = 250;

LedUtility::LedUtility(uint8_t pin){
    _pin = pin;
    _led_state = false;
    _led_i = 0;
    _blinks = 2;
}

void LedUtility::setBlinks(uint16_t amount, uint16_t delay ){
    _blinks = amount;
    _blink_delay = delay;
}

void LedUtility::setBlinks(uint16_t amount){
    setBlinks(amount, 250);
}

void LedUtility::init(){
    pinMode(_pin, OUTPUT);
    ledOn();
    
      xTaskCreatePinnedToCore(
        this->ledUtilityTask,    /* Function to implement the task */
      "ledUtilityTask", /* Name of the task */
      4096,         /* Stack size in words */
      NULL,          /* Task input parameter */
      0,             /* Priority of the task */
      NULL,          /* Task handle. */
      0);     /* Core where the task should run */
    ESP_LOGI("LED UTILITY", "core task OK");

}

void LedUtility::ledOn(){
  digitalWrite(_pin, HIGH);
  _led_state = true;
}

void LedUtility::ledOff(){
  digitalWrite(_pin, LOW);
  _led_state = false;
}

void LedUtility::ledUtilityTask(void *pvParameters){
  while (1){
    if(_blinks != 0){
        while (_led_i < _blinks)
        {
            //ESP_LOGI("LED", "LED state %d",_led_state);
            if(_led_state){
                ledOff();
                _led_i++;
            }else{
                ledOn();
            }
            vTaskDelay(150 / portTICK_PERIOD_MS);
        }
        vTaskDelay(_blink_delay / portTICK_PERIOD_MS);
        _led_i = 0;
    }else{
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }

  }
}
