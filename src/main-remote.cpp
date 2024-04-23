#ifdef REMOTE

#ifdef CORE_50
#include "remote_v50.h"
#endif
#ifdef CORE_43
#include "remote_v43.h"
#endif


#include <math.h>
#include <esp_now.h>
#include "esp_wifi.h"
#include <WiFi.h>
#include <batteryMonitor.h>
#include <ledUtility.h>
#include "esp_log.h"
#include "common.h"

static const char *TAG = "MAIN";
//------------ turn on generic serial printing

packet_t sentData;
packet_t recData;


//---------------------------------------ESP_NOW Variables


String success;
esp_now_peer_info_t peerInfo;
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == 0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recData, incomingData, sizeof(recData));
}




//---------------------------------------HARDWARE DEPENDANT Variables
LedUtility Led(21);

//customisable vars
int analogRes = 10;
int analogReadMax = (1 << analogRes)-1;


//variables for the sketch
int leverValue = 0;

unsigned long currentTime = 0;


void setup() {
  analogReadResolution(analogRes);
  analogSetAttenuation(ADC_11db);
  pinMode(rightBtnPin, INPUT_PULLUP);
  pinMode(leftBtnPin, INPUT_PULLUP);
  pinMode(topBtnPin, INPUT_PULLUP);
  Led.init();
  Led.setBlinks(1,166);
#ifdef DEBUG_PRINTS
  Serial.begin(115200);
  Serial.println("RAC GENERIC BOT");
#endif


  //---------------------------------------ESP NOW setup
  WiFi.enableLongRange(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESPNOW_RAC_CHANNEL,WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, robotAddress, 6);
  peerInfo.channel = ESPNOW_RAC_CHANNEL;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Led.ledOn();
}



void loop() {
  //read pots values
  int stickXValue = analogRead(stickXPotPin);
  delay(3);
  int stickYValue = analogRead(stickYPotPin);
  delay(3);
  int leverValue = analogRead(leverPotPin);
  delay(3);
  currentTime = millis(); 
  bool rightBtnValue = !digitalRead(rightBtnPin);
  bool leftBtnValue = !digitalRead(leftBtnPin);
  bool topBtnValue = !digitalRead(topBtnPin);
  
  // vvvv ----- YOUR AWESOME CODE HERE ----- vvvv //


  // -------------------------------------------- //
  esp_err_t result = -1;
  result = esp_now_send(robotAddress, (uint8_t *) &sentData, sizeof(sentData));
  if (result == ESP_OK) {
    Serial.println("Send ok");
  } else {
    Serial.println("Send fail");
  }
  delay(10);
}
#endif