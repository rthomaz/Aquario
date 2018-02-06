#include "ESPDevice.h"
#include "UnitMeasurementConverter.h"
#include "DisplayManager.h"
#include "DeviceBuzzer.h"
#include "DisplayAccessManager.h"
#include "DisplayWiFiManager.h"
#include "DisplayMQTTManager.h"
#include "DisplayNTPManager.h"
#include "DisplayTemperatureSensorManager.h"
#include "EEPROMManager.h"

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "ESP8266mDNS.h"

//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14 // WiFi Reset Button
#define D6    12 // Antigo DebugManager
#define D7    13
#define D8    15
#define D9    3
#define D10   1

#define HOST_NAME "remotedebug-sample"

#define WEBAPI_HOST  "192.168.1.12"
#define WEBAPI_PORT  80
#define WEBAPI_URI   "/ART.Domotica.WebApi/"

struct config_t
{
  String hardwaresInApplicationId;
  String hardwareId;
} configuration;

int configurationEEPROMAddr = 0;

uint64_t deviceSensorsReadTempTimestamp = 0;
uint64_t deviceSensorsPublishMessageTimestamp = 0;

uint64_t deviceWiFiPublishMessageTimestamp = 0;

using namespace ART;

ESPDevice espDevice(WEBAPI_HOST, WEBAPI_PORT, WEBAPI_URI);

UnitMeasurementConverter unitMeasurementConverter;

DisplayManager displayManager;
DisplayAccessManager displayAccessManager(displayManager);
DisplayWiFiManager displayWiFiManager(displayManager, espDevice);
DisplayMQTTManager displayMQTTManager(displayManager);
DisplayNTPManager displayNTPManager(displayManager, espDevice);
DisplayTemperatureSensorManager displayTemperatureSensorManager(displayManager, espDevice, unitMeasurementConverter);

void setup() {

  Serial.begin(9600);

  // Buzzer
  pinMode(D6, OUTPUT);

  pinMode(D4, INPUT);
  pinMode(D5, INPUT);

  displayManager.begin();

  Serial.println("Iniciando...");

  // text display tests
  displayManager.display.clearDisplay();
  displayManager.display.setTextSize(1);
  displayManager.display.setTextColor(WHITE);
  displayManager.display.setCursor(0, 0);
  displayManager.display.display();

  initConfiguration();

  espDevice.begin();  

  espDevice.getDeviceMQ()->addSubscriptionCallback(mqtt_SubCallback);

  String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");

  if (MDNS.begin(HOST_NAME)) {
    Serial.print("* MDNS responder started. Hostname -> ");
    Serial.println(HOST_NAME);
  }

  WiFi.hostname(hostNameWifi);
}

void initConfiguration()
{
  EEPROM_readAnything(0, configuration);
}

bool mqtt_SubCallback(char* topicKey, char* json)
{
  displayMQTTManager.printReceived(true);
  
  if (espDevice.getDeviceDebug()->isActive(DeviceDebug::DEBUG)) {
    espDevice.getDeviceDebug()->printf("Termometro", "mqtt_SubCallback", "Topic Key: %s\n", topicKey);
  }  

  if (strcmp(topicKey, ESP_DEVICE_UPDATE_PIN_TOPIC_SUB) == 0) {
    displayAccessManager.updatePin(json);
    return true;
  }

  else if (strcmp(topicKey, DEVICE_WIFI_SET_HOST_NAME_TOPIC_SUB) == 0) {
    espDevice.getDeviceWiFi()->setHostName(json);
    return true;
  }
  else if (strcmp(topicKey, DEVICE_WIFI_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB) == 0) {
   espDevice.getDeviceWiFi()->setPublishIntervalInMilliSeconds(json);
   return true;
  }
}

void loop() {

  espDevice.loop(); 

  DeviceInApplication* deviceInApplication = espDevice.getDeviceInApplication();

  if (deviceInApplication->inApplication()) {
      loopInApplication();
  }
  else{
    displayAccessManager.loop();
    //EEPROM_writeAnything(configurationEEPROMAddr, configuration);
  }

  //keep-alive da comunicação com broker MQTT
  espDevice.getDeviceMQ()->loop();

}

void loopInApplication()
{
  displayManager.display.clearDisplay();

  espDevice.getDeviceNTP()->update();

  DeviceSensors* deviceSensors = espDevice.getDeviceSensors();
  bool deviceSensorsRefreshed = deviceSensors->refresh();
  if (deviceSensors->initialized()) {
    displayTemperatureSensorManager.printUpdate(deviceSensorsRefreshed);
  }

  // MQTT
  if(espDevice.getDeviceMQ()->connected()){
    loopMQQTConnected();
  }

  // Wifi
  displayWiFiManager.printSignal();
  
  displayManager.display.display();
}

void loopMQQTConnected()
{
  DeviceMQ* deviceMQ = espDevice.getDeviceMQ();
  DeviceSensors* deviceSensors = espDevice.getDeviceSensors();
  DeviceWiFi* deviceWiFi = espDevice.getDeviceWiFi();
  
  bool mqqtPrintSent = false;
  
  if (deviceSensors->initialized()) {

    displayMQTTManager.printConnected();
    displayMQTTManager.printReceived(false);

    bool deviceSensorsPublished = deviceSensors->publish();

    Serial.printf("deviceSensors->publish: %s\n", deviceSensorsPublished ? "true" : "false");

    if(deviceSensorsPublished){
      mqqtPrintSent = true;
    }    

    //int deviceSensorsPublishIntervalInMilliSeconds = deviceSensors->getPublishIntervalInMilliSeconds();

    uint64_t now = millis();
    
    // Sensor
    displayTemperatureSensorManager.printSensors();

    // Wifi
    
    int deviceWiFiPublishIntervalInMilliSeconds = deviceWiFi->getPublishIntervalInMilliSeconds();

    if (now - deviceWiFiPublishMessageTimestamp > deviceWiFiPublishIntervalInMilliSeconds) {
      
      deviceWiFiPublishMessageTimestamp = now;
      
      mqqtPrintSent = true;

      StaticJsonBuffer<2048> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();

      root["deviceId"] = espDevice.getDeviceId();
      root["deviceDatasheetId"] = espDevice.getDeviceDatasheetId();
      root["wifiQuality"] = espDevice.getDeviceWiFi()->getQuality();
      root["epochTimeUtc"] = espDevice.getDeviceNTP()->getEpochTimeUTC();
      root["localIPAddress"] = espDevice.getDeviceWiFi()->getLocalIPAddress();

      int messageJsonLen = root.measureLength();
      char messageJson[messageJsonLen + 1];
      root.printTo(messageJson, sizeof(messageJson));

      Serial.print("DeviceWiFi enviando para o servidor (Char Len)=> ");
      Serial.println(messageJsonLen);

      deviceMQ->publishInApplication(DEVICE_WIFI_MESSAGE_TOPIC_PUB, messageJson);
    }

    displayMQTTManager.printSent(mqqtPrintSent);
  }
  
}


