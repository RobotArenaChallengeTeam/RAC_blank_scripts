#include "batteryMonitor.h"
#include "esp_log.h"
uint8_t BatteryMonitor::_pin;
bool BatteryMonitor::_low;


BatteryMonitor::BatteryMonitor(int pin)
{
  _pin = pin;
}

float BatteryMonitor::getVoltage()
{
  int batteryLevel = analogRead(_pin);
  float batteryVoltage = (float)(-0.00000450923576988579 * batteryLevel * batteryLevel) + 0.014775524861582 * batteryLevel - 1.80504567581238;
  return batteryVoltage;
}

bool BatteryMonitor::isLow(){
  return _low;
}

void BatteryMonitor::init()
{
  pinMode(_pin, INPUT);
  xTaskCreatePinnedToCore(
      this->batteryMonitorTask, /* Function to implement the task */
      "batteryMonitorTask", /* Name of the task */
      4096,                 /* Stack size in words */
      NULL,                 /* Task input parameter */
      0,                    /* Priority of the task */
      NULL,                 /* Task handle. */
      0);                   /* Core where the task should run */
  ESP_LOGI("BATTERY MONITOR", "core task OK");
}

void BatteryMonitor::batteryMonitorTask(void *pvParameters)
{
  for (;;)
  {
    float batteryLevel = getVoltage();
    if (batteryLevel < 6.8)
    {
      _low = true;
    }
    else
    {
      _low = false;
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}