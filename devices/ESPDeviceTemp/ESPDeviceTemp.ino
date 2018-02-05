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

	espDevice.getDeviceSensors()->begin();

	espDevice.getDeviceMQ()->addSubscriptionCallback(mqtt_SubCallback);
  espDevice.getDeviceMQ()->addConnectedInApplicationCallback(subscribeInApplication);

  espDevice.getDeviceMQ()->begin();

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

void unSubscribeNotInApplication()
{
	Serial.println("[MQQT::unSubscribeNotInApplication] initializing ...");

	espDevice.getDeviceMQ()->unSubscribeInDevice(ESP_DEVICE_UPDATE_PIN_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInDevice(ESP_DEVICE_INSERT_IN_APPLICATION_TOPIC_SUB);

	Serial.println("[MQQT::unSubscribeNotInApplication] Initialized with success !");
}

void subscribeInApplication()
{
	Serial.println("[MQQT::subscribeInApplication] initializing ...");

	espDevice.getDeviceMQ()->subscribeInDevice(ESP_DEVICE_DELETE_FROM_APPLICATION_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(ESP_DEVICE_SET_LABEL_TOPIC_SUB);

	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_NTP_SET_UTC_TIME_OFF_SET_IN_SECOND_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_NTP_SET_UPDATE_INTERVAL_IN_MILLI_SECOND_TOPIC_SUB);

	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_WIFI_SET_HOST_NAME_TOPIC_SUB);
  espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_WIFI_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);

	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_REMOTE_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_RESET_CMD_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_SERIAL_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_SHOW_COLORS_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_SHOW_DEBUG_LEVEL_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_SHOW_PROFILER_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_DEBUG_SET_SHOW_TIME_TOPIC_SUB);

  espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_SENSORS_GET_FULL_BY_DEVICE_IN_APPLICATION_ID_COMPLETED_TOPIC_SUB); 
	espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_SENSORS_SET_READ_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);
  espDevice.getDeviceMQ()->subscribeInApplication(DEVICE_SENSORS_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);

  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_IN_DEVICE_SET_ORDINATION_TOPIC_SUB);

	espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_SET_LABEL_TOPIC_SUB);

	espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TEMP_DS_FAMILY_SET_RESOLUTION_TOPIC_SUB);
  
  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TRIGGER_INSERT_TOPIC_SUB);  
  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TRIGGER_DELETE_TOPIC_SUB);  
	espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TRIGGER_SET_TRIGGER_ON_TOPIC_SUB);	
	espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TRIGGER_SET_BUZZER_ON_TOPIC_SUB);
  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_TRIGGER_SET_TRIGGER_VALUE_TOPIC_SUB);

  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_SET_DATASHEET_UNIT_MEASUREMENT_SCALE_TOPIC_SUB);
	espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_RANGE_SET_VALUE_TOPIC_SUB);
  espDevice.getDeviceMQ()->subscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_CHART_LIMITER_SET_VALUE_TOPIC_SUB);

	Serial.println("[MQQT::subscribeInApplication] Initialized with success !");
}

void unSubscribeInApplication()
{
	Serial.println("[MQQT::unSubscribeInApplication] initializing ...");

	espDevice.getDeviceMQ()->unSubscribeInDevice(ESP_DEVICE_DELETE_FROM_APPLICATION_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(ESP_DEVICE_SET_LABEL_TOPIC_SUB);

	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_NTP_SET_UTC_TIME_OFF_SET_IN_SECOND_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_NTP_SET_UPDATE_INTERVAL_IN_MILLI_SECOND_TOPIC_SUB);

	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_WIFI_SET_HOST_NAME_TOPIC_SUB);
  espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_WIFI_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);
  
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_REMOTE_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_RESET_CMD_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_SERIAL_ENABLED_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_SHOW_COLORS_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_SHOW_DEBUG_LEVEL_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_SHOW_PROFILER_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_DEBUG_SET_SHOW_TIME_TOPIC_SUB);

	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_SENSORS_GET_FULL_BY_DEVICE_IN_APPLICATION_ID_COMPLETED_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_SENSORS_SET_READ_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);
  espDevice.getDeviceMQ()->unSubscribeInApplication(DEVICE_SENSORS_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB);

  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_IN_DEVICE_SET_ORDINATION_TOPIC_SUB);
  
	espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_SET_LABEL_TOPIC_SUB);
	
	espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TEMP_DS_FAMILY_SET_RESOLUTION_TOPIC_SUB);

  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TRIGGER_INSERT_TOPIC_SUB);  
  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TRIGGER_DELETE_TOPIC_SUB);  
	espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TRIGGER_SET_TRIGGER_ON_TOPIC_SUB);	
	espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TRIGGER_SET_BUZZER_ON_TOPIC_SUB);
  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_TRIGGER_SET_TRIGGER_VALUE_TOPIC_SUB);

  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_SET_DATASHEET_UNIT_MEASUREMENT_SCALE_TOPIC_SUB);
	espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_RANGE_SET_VALUE_TOPIC_SUB);
  espDevice.getDeviceMQ()->unSubscribeInApplication(SENSOR_UNIT_MEASUREMENT_SCALE_CHART_LIMITER_SET_VALUE_TOPIC_SUB);

	Serial.println("[MQQT::unSubscribeInApplication] Initialized with success !");
}

void mqtt_SubCallback(char* topicKey, char* json)
{
  if (espDevice.getDeviceDebug()->isActive(DeviceDebug::DEBUG)) {
	  espDevice.getDeviceDebug()->printf("Termometro", "mqtt_SubCallback", "Topic Key: %s\n", topicKey);
  }
  
	displayMQTTManager.printReceived(true);

	if (strcmp(topicKey, ESP_DEVICE_UPDATE_PIN_TOPIC_SUB) == 0) {
		displayAccessManager.updatePin(json);
	}
	if (strcmp(topicKey, ESP_DEVICE_INSERT_IN_APPLICATION_TOPIC_SUB) == 0) {
		unSubscribeNotInApplication();
		espDevice.getDeviceInApplication()->insertInApplication(json);
		subscribeInApplication();
	}
	if (strcmp(topicKey, ESP_DEVICE_DELETE_FROM_APPLICATION_TOPIC_SUB) == 0) {
		unSubscribeInApplication();
		espDevice.getDeviceInApplication()->deleteFromApplication();
		//TODO:voltar subscribeNotInApplication();
	}
	if (strcmp(topicKey, ESP_DEVICE_SET_LABEL_TOPIC_SUB) == 0) {
		espDevice.setLabel(json);
	}

	if (strcmp(topicKey, DEVICE_NTP_SET_UTC_TIME_OFF_SET_IN_SECOND_TOPIC_SUB) == 0) {
		espDevice.getDeviceNTP()->setUtcTimeOffsetInSecond(json);
	}
	if (strcmp(topicKey, DEVICE_NTP_SET_UPDATE_INTERVAL_IN_MILLI_SECOND_TOPIC_SUB) == 0) {
		espDevice.getDeviceNTP()->setUpdateIntervalInMilliSecond(json);
	}

	if (strcmp(topicKey, DEVICE_WIFI_SET_HOST_NAME_TOPIC_SUB) == 0) {
		espDevice.getDeviceWiFi()->setHostName(json);
	}
  if (strcmp(topicKey, DEVICE_WIFI_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB) == 0) {
   espDevice.getDeviceWiFi()->setPublishIntervalInMilliSeconds(json);
  }

	if (strcmp(topicKey, DEVICE_DEBUG_SET_REMOTE_ENABLED_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setRemoteEnabled(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_RESET_CMD_ENABLED_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setResetCmdEnabled(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_SERIAL_ENABLED_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setSerialEnabled(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_SHOW_COLORS_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setShowColors(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_SHOW_DEBUG_LEVEL_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setShowDebugLevel(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_SHOW_PROFILER_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setShowProfiler(json);
	}
	if (strcmp(topicKey, DEVICE_DEBUG_SET_SHOW_TIME_TOPIC_SUB) == 0) {
		espDevice.getDeviceDebug()->setShowTime(json);
	}

  if (strcmp(topicKey, DEVICE_SENSORS_GET_FULL_BY_DEVICE_IN_APPLICATION_ID_COMPLETED_TOPIC_SUB) == 0) {
    espDevice.getDeviceSensors()->setSensorsByMQQTCallback(json);
  }
	if (strcmp(topicKey, DEVICE_SENSORS_SET_READ_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setReadIntervalInMilliSeconds(json);
	}
  if (strcmp(topicKey, DEVICE_SENSORS_SET_PUBLISH_INTERVAL_IN_MILLI_SECONDS_TOPIC_SUB) == 0) {
   espDevice.getDeviceSensors()->setPublishIntervalInMilliSeconds(json);
  }

  if (strcmp(topicKey, SENSOR_IN_DEVICE_SET_ORDINATION_TOPIC_SUB) == 0) {
   espDevice.getDeviceSensors()->setOrdination(json);
  }

	if (strcmp(topicKey, SENSOR_SET_LABEL_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setLabel(json);
	}
	
	if (strcmp(topicKey, SENSOR_TEMP_DS_FAMILY_SET_RESOLUTION_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setResolution(json);
	}
 
  if (strcmp(topicKey, SENSOR_TRIGGER_INSERT_TOPIC_SUB) == 0) {
   espDevice.getDeviceSensors()->insertTrigger(json);
  } 
  if (strcmp(topicKey, SENSOR_TRIGGER_DELETE_TOPIC_SUB) == 0) {
    espDevice.getDeviceSensors()->deleteTrigger(json);
  } 
	if (strcmp(topicKey, SENSOR_TRIGGER_SET_TRIGGER_ON_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setTriggerOn(json);
	}	
	if (strcmp(topicKey, SENSOR_TRIGGER_SET_BUZZER_ON_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setBuzzerOn(json);
	}
  if (strcmp(topicKey, SENSOR_TRIGGER_SET_TRIGGER_VALUE_TOPIC_SUB) == 0) {
    espDevice.getDeviceSensors()->setTriggerValue(json);
  }
  
  if (strcmp(topicKey, SENSOR_UNIT_MEASUREMENT_SCALE_SET_DATASHEET_UNIT_MEASUREMENT_SCALE_TOPIC_SUB) == 0) {
    espDevice.getDeviceSensors()->setDatasheetUnitMeasurementScale(json);
  }
	if (strcmp(topicKey, SENSOR_UNIT_MEASUREMENT_SCALE_RANGE_SET_VALUE_TOPIC_SUB) == 0) {
		espDevice.getDeviceSensors()->setRange(json);
	}
  if (strcmp(topicKey, SENSOR_UNIT_MEASUREMENT_SCALE_CHART_LIMITER_SET_VALUE_TOPIC_SUB) == 0) {
    espDevice.getDeviceSensors()->setChartLimiter(json);
  } 
}

void loop() {

	espDevice.loop();	

	DeviceInApplication* deviceInApplication = espDevice.getDeviceInApplication();

	if (deviceInApplication != NULL && deviceInApplication->getApplicationId() == NULL) {
		displayAccessManager.loop();
		//EEPROM_writeAnything(configurationEEPROMAddr, configuration);
	}
	else {
		loopInApplication();
	}

	//keep-alive da comunicação com broker MQTT
	espDevice.getDeviceMQ()->loop();

}

void loopInApplication()
{
	displayManager.display.clearDisplay();

	espDevice.getDeviceNTP()->update();

	uint64_t now = millis();

  DeviceSensors* deviceSensors = espDevice.getDeviceSensors();
  DeviceWiFi* deviceWiFi = espDevice.getDeviceWiFi();
  
	if (deviceSensors->initialized()) {
		if (now - deviceSensorsReadTempTimestamp > deviceSensors->getReadIntervalInMilliSeconds()) {
			deviceSensorsReadTempTimestamp = now;
			displayTemperatureSensorManager.printUpdate(true);
			deviceSensors->refresh();
		}
		else {
			displayTemperatureSensorManager.printUpdate(false);
		}
	}

	// MQTT
  if(espDevice.getDeviceMQ()->connected()){
    loopMQQTConnected(now);
  }

	// Wifi
	displayWiFiManager.printSignal();
  
	displayManager.display.display();
}

void loopMQQTConnected(uint64_t now)
{
  DeviceMQ* deviceMQ = espDevice.getDeviceMQ();
  DeviceSensors* deviceSensors = espDevice.getDeviceSensors();
  DeviceWiFi* deviceWiFi = espDevice.getDeviceWiFi();
  
  bool mqqtPrintSent = false;
  
  if (deviceSensors->initialized()) {

    displayMQTTManager.printConnected();
    displayMQTTManager.printReceived(false);

    int deviceSensorsPublishIntervalInMilliSeconds = deviceSensors->getPublishIntervalInMilliSeconds();

    if (now - deviceSensorsPublishMessageTimestamp > deviceSensorsPublishIntervalInMilliSeconds) {
      
      deviceSensorsPublishMessageTimestamp = now;
      
      mqqtPrintSent = true;

      StaticJsonBuffer<2048> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();

      root["deviceId"] = espDevice.getDeviceId();
      root["deviceDatasheetId"] = espDevice.getDeviceDatasheetId();
      root["epochTimeUtc"] = espDevice.getDeviceNTP()->getEpochTimeUTC();

      espDevice.getDeviceSensors()->createSensorsJsonNestedArray(root);

      int messageJsonLen = root.measureLength();
      char messageJson[messageJsonLen + 1];
      root.printTo(messageJson, sizeof(messageJson));

      Serial.print("DeviceSensors enviando para o servidor (Char Len)=> ");
      Serial.println(messageJsonLen);

      deviceMQ->publishInApplication(DEVICE_SENSORS_MESSAGE_TOPIC_PUB, messageJson);
    }

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

