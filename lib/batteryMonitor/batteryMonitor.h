#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

class BatteryMonitor {
  public:
    BatteryMonitor(int pin = 9);
    void init();
    static float getVoltage();
    bool isLow();
    static void batteryMonitorTask(void *pvParameters);

  private:
    static uint8_t _pin;
    static bool _low;

};

#endif
