#ifdef ROBOT
#include <esp_now.h>
#include "esp_wifi.h"
#include <WiFi.h>
#include <motorControl.h>
#include <batteryMonitor.h>
#include <ledUtility.h>
#include "esp_log.h"
#include "common.h"
static const char *TAG = "MAIN";

#define weapPot 7

//------------ turn on generic serial printing
//#define DEBUG_PRINTS


#define MOTOR_A_IN1 16
#define MOTOR_A_IN2 17

#define MOTOR_B_IN1 8
#define MOTOR_B_IN2 18

#define MOTOR_C_IN1 4
#define MOTOR_C_IN2 5


//RIGHT
MotorControl motor1 = MotorControl(MOTOR_A_IN1, MOTOR_A_IN2); 
//LEFT
MotorControl motor2 = MotorControl(MOTOR_B_IN1, MOTOR_B_IN2);
//WPN
MotorControl motor3 = MotorControl(MOTOR_C_IN1, MOTOR_C_IN2);

BatteryMonitor Battery = BatteryMonitor();

LedUtility Led = LedUtility();

packet_t recData;


bool failsafe = false;
unsigned long failsafeMaxMillis = 400;
unsigned long lastPacketMillis = 0;

int recLpwm = 0;
int recRpwm = 0;
int recArg1 = 0;
int recArg2 = 0;
int recArg3 = 0;


// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&recData, incomingData, sizeof(recData));
  recLpwm = recData.speedmotorLeft;
  recRpwm = recData.speedmotorRight;
  recArg1 = recData.packetArg1;
  recArg2 = recData.packetArg2;
  recArg3 = recData.packetArg3;
  lastPacketMillis = millis();
  failsafe = false;
}

int handle_blinks(){
  if(Battery.isLow()){
    Led.setBlinks(1,500,2.0);
    return 1;
  }
  if (failsafe){
    Led.setBlinks(2,500);
    return -1;
  }
  Led.ledOn();
  return 0;
}

void setup()
{
#ifdef DEBUG_PRINTS
  Serial.begin(115200);
  Serial.println("Ready.");
#endif
  analogReadResolution(10);
  Led.init();
  Led.setBlinks(1,166);
  delay(2000);
  Battery.init();
  delay(20);


  analogSetAttenuation(ADC_11db);
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  delay(500);

  WiFi.enableLongRange(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_RAC_CHANNEL,WIFI_SECOND_CHAN_NONE);
  if (esp_wifi_set_mac(WIFI_IF_STA, &robotAddress[0]) != ESP_OK)
  {
    Serial.println("Error changing mac");
    return;
  }
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Led.ledOn();
}

void loop()
{

  unsigned long current_time = millis();
  if (current_time - lastPacketMillis > failsafeMaxMillis)
  {
    failsafe = true;
  }
  handle_blinks();
  if (failsafe)
  {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    motor3.setSpeed(0);
  }
  else
  {
  // vvvv ----- YOUR AWESOME CODE HERE ----- vvvv //



  // -------------------------------------------- //
  }
  delay(2);
}
#endif