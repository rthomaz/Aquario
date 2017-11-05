#include "Arduino.h"
#include "DebugManager.h"
#include "AccessManager.h"
#include "TemperatureSensorManager.h"
#include "NTPManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "BuzzerManager.h"
#include "DisplayAccessManager.h"
#include "DisplayWiFiManager.h"
#include "DisplayMQTTManager.h"
#include "DisplayNTPManager.h"
#include "DisplayTemperatureSensorManager.h"
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "ArduinoJson.h"
#include "EEPROMManager.h"

//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

#define HOST  "192.168.1.12"
#define PORT  80
#define URI   "/ART.Domotica.WebApi/"

struct config_t
{
    String hardwaresInApplicationId;
    String hardwareId;
} configuration;

int configurationEEPROMAddr = 0;

#define TOPIC_SUB_UPDATE_PIN "ESPDevice.UpdatePin"
#define TOPIC_SUB_SET_RESOLUTION "DSFamilyTempSensor.SetResolution"
#define TOPIC_SUB_SET_HIGH_ALARM "DSFamilyTempSensor.SetHighAlarm"
#define TOPIC_SUB_SET_LOW_ALARM "DSFamilyTempSensor.SetLowAlarm"
#define TOPIC_SUB_INSERT_IN_APPLICATION "ESPDevice.InsertInApplication"
#define TOPIC_SUB_DELETE_FROM_APPLICATION "ESPDevice.DeleteFromApplication"

#define TOPIC_SUB_GET_IN_APPLICATION_FOR_DEVICE_COMPLETED   "ESPDevice.GetInApplicationForDeviceCompleted"    //tópico MQTT de envio de informações para Broker

#define TOPIC_PUB_GET_IN_APPLICATION_FOR_DEVICE   "ESPDevice.GetInApplicationForDevice"    //tópico MQTT de envio de informações para Broker
#define TOPIC_PUB_TEMP   "ARTPUBTEMP"    //tópico MQTT de envio de informações para Broker

#define MESSAGE_INTERVAL 4000
uint64_t messageTimestamp = 0;

#define READTEMP_INTERVAL 2000
uint64_t readTempTimestamp = 0;

DebugManager debugManager(D6);
WiFiManager wifiManager(D5, debugManager);
AccessManager accessManager(debugManager, wifiManager, HOST, PORT, URI);
NTPManager ntpManager(debugManager);
DisplayManager displayManager(debugManager);
TemperatureSensorManager temperatureSensorManager(debugManager, ntpManager);
BuzzerManager buzzerManager(D7, debugManager);

DisplayAccessManager displayAccessManager(debugManager, displayManager);
DisplayWiFiManager displayWiFiManager(displayManager, wifiManager, debugManager);
DisplayMQTTManager displayMQTTManager(displayManager, debugManager);
DisplayNTPManager displayNTPManager(displayManager, ntpManager, debugManager);
DisplayTemperatureSensorManager displayTemperatureSensorManager(displayManager, temperatureSensorManager, debugManager);

WiFiClient espClient;
PubSubClient MQTT(espClient);

bool _mqttInitialized = false;

void setup() {
		
	Serial.begin(9600);

  // Buzzer
  pinMode(D6,OUTPUT);

  pinMode(D4, INPUT);
  pinMode(D5, INPUT);  

	debugManager.update();  

	displayManager.begin();

	temperatureSensorManager.begin();

	if (debugManager.isDebug()) Serial.println("Iniciando...");

	// text display tests
	displayManager.display.clearDisplay();
	displayManager.display.setTextSize(1);
	displayManager.display.setTextColor(WHITE);
	displayManager.display.setCursor(0, 0);	
	displayManager.display.display();

  //TODO: Gambeta, nos constructors estão comentados os códigos que deveriam funcionar no lugar destes handlers abaixo
	wifiManager.setStartConfigPortalCallback(handleWMStartConfigPortalCallback);
  wifiManager.setCaptivePortalCallback(handleWMCaptivePortalCallback);
  wifiManager.setSuccessConfigPortalCallback(handleWMSuccessConfigPortalCallback);    
  wifiManager.setFailedConfigPortalCallback(handleWMFailedConfigPortalCallback);    
  wifiManager.setConnectingConfigPortalCallback(handleWMConnectingConfigPortalCallback); 
  ntpManager.setUpdateCallback(handleNTPUpdateCallback);  
  
  wifiManager.autoConnect();

  initConfiguration();

  accessManager.begin();  
  
  initMQTT();

  ntpManager.begin();  
}

//TODO: Gambeta, nos constructors estão comentados os códigos que deveriam funcionar no lugar destes handlers abaixo
void handleWMStartConfigPortalCallback () {  displayWiFiManager.startConfigPortalCallback(); }
void handleWMCaptivePortalCallback (String ip) {  displayWiFiManager.captivePortalCallback(ip); }
void handleWMSuccessConfigPortalCallback () {  displayWiFiManager.successConfigPortalCallback(); }
void handleWMFailedConfigPortalCallback (int connectionResult) {  displayWiFiManager.failedConfigPortalCallback(connectionResult); }
void handleWMConnectingConfigPortalCallback () {  displayWiFiManager.connectingConfigPortalCallback(); }
void handleNTPUpdateCallback(bool update, bool forceUpdate){ displayNTPManager.updateCallback(update, forceUpdate); }

void initConfiguration()
{
  EEPROM_readAnything(0, configuration);
}

void initMQTT() 
{
    if(wifiManager.isConnected() && accessManager.initialized()){
      char* const brokerHost = strdup(accessManager.getBrokerHost().c_str());
      int brokerPort = accessManager.getBrokerPort();
      
      MQTT.setServer(brokerHost, brokerPort);   //informa qual broker e porta deve ser conectado
      MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega) 

      _mqttInitialized = true;

      Serial.println("[MQQT] Initialized with success !");
    }
    else{
      _mqttInitialized = false;

      Serial.println("[MQQT] Not initialized");
    }    
}
 
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    displayMQTTManager.printReceived(true);
    
    String json;
    
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       json += c;
    }

    Serial.print("payload: ");
    Serial.println(json);

    StaticJsonBuffer<300> jsonBuffer;
    
    JsonObject& root = jsonBuffer.parseObject(json);
  
    if (!root.success()) {
      Serial.print("parse payload failed: ");
      Serial.println(json);
      return;
    }

    String payloadTopic = root["topic"];
    String payloadContract = root["contract"];

    Serial.print("payloadTopic: ");
    Serial.println(payloadTopic);

    Serial.print("payloadContract: ");
    Serial.println(payloadContract);

    if(payloadTopic == String(TOPIC_SUB_UPDATE_PIN)){
      displayAccessManager.updatePin(payloadContract);
    }
    if(payloadTopic == String(TOPIC_SUB_GET_IN_APPLICATION_FOR_DEVICE_COMPLETED)){
      getInApplicationForDeviceCompleted();      
    }
    if(payloadTopic == String(TOPIC_SUB_SET_RESOLUTION)){
      temperatureSensorManager.setResolution(payloadContract);
    }
    if(payloadTopic == String(TOPIC_SUB_SET_HIGH_ALARM)){
      temperatureSensorManager.setHighAlarm(payloadContract);
    }
    if(payloadTopic == String(TOPIC_SUB_SET_LOW_ALARM)){
      temperatureSensorManager.setLowAlarm(payloadContract);
    }
    if(payloadTopic == String(TOPIC_SUB_INSERT_IN_APPLICATION)){
      Serial.println("INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT INSERT ");
    }
    if(payloadTopic == String(TOPIC_SUB_DELETE_FROM_APPLICATION)){
      Serial.println("DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE ");
    }
}

void getInApplicationForDevice()
{
  StaticJsonBuffer<200> JSONbuffer;
  JsonObject& root = JSONbuffer.createObject();
  root["chipId"] = ESP.getChipId();
  root["flashChipId"] = ESP.getFlashChipId();
  root["macAddress"] = WiFi.macAddress();
  
  int len = root.measureLength();
  char result[len + 1];
  root.printTo(result, sizeof(result));
  
  MQTT.publish(TOPIC_PUB_GET_IN_APPLICATION_FOR_DEVICE, result);    
}

void getInApplicationForDeviceCompleted()
{
  Serial.println("******************** TOPIC_SUB_GET_IN_APPLICATION_FOR_DEVICE_COMPLETED ******************** !!!!!!!!!!!!!!!!!!!!!!!!!");
}

void reconnectMQTT() 
{
    if(!wifiManager.isConnected() || !accessManager.initialized()){
      return;
    }
    
    if(!_mqttInitialized){
      initMQTT();
    }
    
    if (!MQTT.connected()) 
    {
        char* const brokerHost = strdup(accessManager.getBrokerHost().c_str());
        char* const brokerUser = strdup(accessManager.getBrokerUser().c_str());
        char* const brokerPwd  = strdup(accessManager.getBrokerPwd().c_str());
        char* const clientId  = strdup(accessManager.getHardwareId().c_str());
        
        Serial.print("[MQQT] Tentando se conectar ao Broker MQTT: ");
        Serial.println(brokerHost);

        Serial.print("[MQQT] ClientId: ");
        Serial.println(clientId);        
        
        Serial.print("[MQQT] User: ");
        Serial.println(brokerUser);        

        Serial.print("[MQQT] Pwd: ");
        Serial.println(brokerPwd);        

        byte willQoS = 0;
        const char* willTopic = "willTopic";
        const char* willMessage = "My Will Message";
        boolean willRetain = false;
        
        //if (MQTT.connect(clientId, brokerUser, brokerPwd)) 
        if (MQTT.connect(clientId, brokerUser, brokerPwd, willTopic, willQoS, willRetain, willMessage)) 
        {
            Serial.println("[MQQT] Conectado com sucesso ao broker MQTT!");

            MQTT.subscribe(TOPIC_SUB_UPDATE_PIN); 
            //MQTT.subscribe(TOPIC_SUB_SET_RESOLUTION); 
            //MQTT.subscribe(TOPIC_SUB_SET_HIGH_ALARM); 
            //MQTT.subscribe(TOPIC_SUB_SET_LOW_ALARM);    
            //MQTT.subscribe(TOPIC_SUB_GET_IN_APPLICATION_FOR_DEVICE_COMPLETED);                
        } 
        else 
        {
            Serial.println("[MQQT] Falha ao reconectar no broker.");
            Serial.println("[MQQT] Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void loop() {	  

  debugManager.update();      

  wifiManager.autoConnect(); //se não há conexão com o WiFI, a conexão é refeita
  accessManager.autoInitialize(); 
  reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
  
  if(accessManager.getHardwareInApplicationId() == ""){
    displayAccessManager.loop();
    //EEPROM_writeAnything(configurationEEPROMAddr, configuration);
    //getInApplicationForDevice();
  }    
  else{
    loopInApplication();  
  }  
    
  //keep-alive da comunicação com broker MQTT
  MQTT.loop();  
  
}

void loopInApplication()
{
  displayManager.display.clearDisplay();
  
  ntpManager.update();
  
  uint64_t now = millis();   

  if(now - readTempTimestamp > READTEMP_INTERVAL) {
    readTempTimestamp = now;
    displayTemperatureSensorManager.printUpdate(true);
    temperatureSensorManager.refresh();
  }  
  else{
    displayTemperatureSensorManager.printUpdate(false);
  }
  
  // MQTT
  if(MQTT.connected()){
    
    displayMQTTManager.printConnected();  
    displayMQTTManager.printReceived(false);
    
    if(now - messageTimestamp > MESSAGE_INTERVAL) {
      messageTimestamp = now;
      displayMQTTManager.printSent(true);
      char *sensorsJson = temperatureSensorManager.getSensorsJson();
      Serial.println("enviando para o servidor => ");
      //Serial.println(sensorsJson); // está estourando erro aqui
      MQTT.publish(TOPIC_PUB_TEMP, sensorsJson);            
    } 
    else {
      displayMQTTManager.printSent(false);
    }
         
  }       

  // Wifi
  displayWiFiManager.printSignal();
    
  // Sensor
  displayTemperatureSensorManager.printSensors();  
  
  // Buzzer
  //buzzerManager.test();

  displayManager.display.display();
}

