#include <math.h>
#include <esp_now.h>
#include "esp_wifi.h"
#include <WiFi.h>
#include "esp_log.h"

static const char *TAG = "MAIN";
//------------ turn on generic serial printing
#define ESPNOW_RAC_CHANNEL 2

// note, the first number cannot be odd due to wssp mac limitation
//possible first number can only end in  0,2,4,6,8,A,C,E, 
//ex: 0x1A is valid, 0xF3 is invalid
uint8_t robotAddress[] = {0xF8, 0x9F, 0x00, 0x8F, 0x00, 0x70};

//PACKETS
// remote -> robot
typedef struct {
  int16_t motor_left;
  int16_t motor_right;
  int16_t arm_position;
  int16_t arm_trigger;
  int16_t bonus_value;
} packet_t;

packet_t robot_data;
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
void OnDataRecv(const esp_now_recv_info *mac, const uint8_t *incomingData, int len) {
  memcpy(&recData, incomingData, sizeof(recData));
}




//---------------------------------------HARDWARE DEPENDANT Variables
//latest version
#ifndef OLD_REMOTE
#define right_left_pin 7
#define forwd_backwd_pin 2
#define lever_pin 6

#define r_btn_pin 1
#define l_btn_pin 4
#define a_btn_pin 5
#endif

#ifdef OLD_REMOTE
#define right_left_pin 7
#define forwd_backwd_pin 10
#define lever_pin 8

#define r_btn_pin 2
#define l_btn_pin 4
#define a_btn_pin 5
#endif

//customisable vars
int analogRes = 10;
int analogReadMax = (1 << analogRes)-1;
//variables for the sketch
unsigned long current_time=0;
void setup() {
  analogReadResolution(analogRes);
  analogSetAttenuation(ADC_11db);
  pinMode(r_btn_pin, INPUT_PULLUP);
  pinMode(l_btn_pin, INPUT_PULLUP);
  pinMode(a_btn_pin, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("RAC GENERIC BOT");



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
  esp_now_register_recv_cb(OnDataRecv);\
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
}

void loop() {
  //read pots values
  int right_left_val = analogRead(right_left_pin);
  delay(3);
  int forwd_backwd_val = analogRead(forwd_backwd_pin);
  delay(3);
  int lever_val = analogRead(lever_pin);
  delay(3);
  current_time = millis(); 
  bool r_btn_val = !digitalRead(r_btn_pin);
  bool l_btn_val = !digitalRead(l_btn_pin);
  bool a_btn_val = !digitalRead(a_btn_pin);
  robot_data.motor_right = 0;
  robot_data.motor_left = 0;
  robot_data.arm_position = 0;
  
  // vvvv ----- YOUR AWESOME CODE HERE ----- vvvv //
  if(a_btn_val == true){
    robot_data.motor_right = -512;
    robot_data.motor_left = -512;
  }
  if(r_btn_val == true){
    robot_data.motor_right = 512;
  }
  if(l_btn_val == true){
    robot_data.motor_left = 512;
  }
  // -------------------------------------------- //
  Serial.print("R: ");
  Serial.print(robot_data.motor_right);
  Serial.print(" L: ");
  Serial.print(robot_data.motor_left);
  Serial.print(" A: ");
  Serial.print(robot_data.arm_position);

  esp_err_t result = -1;
  result = esp_now_send(robotAddress, (uint8_t *) &robot_data, sizeof(robot_data));
  if (result == ESP_OK) {
    Serial.println("Send ok");
  } else {
    Serial.println("Send fail");
  }
  delay(10);
}