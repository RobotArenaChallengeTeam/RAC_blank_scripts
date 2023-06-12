#ifndef LED_UTILITY_H
#define LED_UTILITY_H
#include <Arduino.h>

class LedUtility
{
public:
    LedUtility(uint8_t pin = 21);
    void init();
    static void setBlinks(uint16_t amount, uint16_t delay);
    static void setBlinks(uint16_t amount);
    static void ledOn();
    static void ledOff();
    static void ledUtilityTask(void *pvParameters);
    
 private:
    static uint8_t _pin;
    static bool _led_state;
    static int16_t _led_i;
    static int16_t _blinks;
    static  uint16_t _blink_delay;

};

#endif
