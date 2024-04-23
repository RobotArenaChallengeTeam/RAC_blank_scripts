#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>
/**
     * @brief create a battery monitor object to handle battery voltage readings
     * in a separate core task
     * @attention 1. requires init() on setup to work
     * @attention 2. pin must have adc capabilites
     * @attention 3. to avoid polling the voltage is check only once each 2 seconds
     * @attention 4. the reading are calibrated with a voltage divider 47kohm and 27kohm
     * 
     * @param pin  pin the voltage divider is attached to, default 9
     */
class BatteryMonitor {
  public:
    BatteryMonitor(int pin = 9);
    /**
     * @brief start the battery monitor coroutine
     *
     */
    void init();

    /**
     * @brief set the voltage threshold, when a reading will be under this value 
     * the battery will be considered at low voltage
     *
     * @param threshold the voltage in volts, default 6.8V
     */
    void setVoltageThreshold(float threshold = 6.8);
    /**
     * @brief get the current voltage
     *
     * @return voltage in volts
     */
    static float getVoltage();

    /**
     * @brief check if the battery is under the low voltage threshold
     *
     * @return true if last checked voltage was under the threshold
     */
    bool isLow();

    static void batteryMonitorTask(void *pvParameters);

  private:
    static uint8_t _pin;
    static bool _low;
    static float _low_voltage_threshold;
};

#endif
