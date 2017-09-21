/*
* WebSocketClient.ino
*
*  Created on: 24.05.2015
*
*/

#include <WifiManager.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <WebSocketsClient.h>

#include <Hash.h>

#include <TemperatureSensorManager.h>

const char* host = "Termometro";
const char* ssid = "RThomaz";
const char* password = "2919517400";

IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebSocketsClient webSocket;

#define MESSAGE_INTERVAL 3000
#define HEARTBEAT_INTERVAL 25000

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

//NTP inicio

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;

int ntpUpdateInterval = 60000;
int16_t utc = 0; //UTC

NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc * 3600, ntpUpdateInterval);

//NTP fim

#include <DisplayManager.h>

DisplayManager displayManager;

#include <WifiManager.h>

WifiManager wifiManager;

TemperatureSensorManager temperatureSensorManager(timeClient);

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	Serial.printf("webSocketEvent!!!");

	switch (type) {
	case WStype_DISCONNECTED:
		Serial.printf("[WSc] Disconnected!\n");
		isConnected = false;
		break;
	case WStype_CONNECTED:
	{
		Serial.printf("[WSc] Connected to url: %s\n", payload);
		isConnected = true;

		// send message to server when Connected
		// socket.io upgrade confirmation message (required)
		webSocket.sendTXT("5");
	}
	break;
	case WStype_TEXT:
		Serial.printf("[WSc] get text: %s\n", payload);

		// send message to server
		// webSocket.sendTXT("message here");
		break;
	case WStype_BIN:
		Serial.printf("[WSc] get binary length: %u\n", length);
		hexdump(payload, length);

		// send data to server
		// webSocket.sendBIN(payload, length);
		break;
	}

}

void setup() {

	Serial.begin(9600);

	WiFi.config(ip, gateway, subnet);

	WiFi.mode(WIFI_AP_STA);
	WiFi.begin(ssid, password);

	displayManager.begin();
	temperatureSensorManager.begin();

	Serial.println("Iniciando...");

	// text display tests
	displayManager.display.clearDisplay();
	displayManager.display.setTextSize(1);
	displayManager.display.setTextColor(WHITE);
	displayManager.display.setCursor(0, 0);

	displayManager.display.println("Conectando Wifi...");
	displayManager.display.display();

	if (WiFi.waitForConnectResult() == WL_CONNECTED) {

		Serial.println("conectou wifi!");

		displayManager.display.println("Wifi conectado !!!");
		displayManager.display.display();
		delay(2000);

		webSocket.beginSocketIO("192.168.1.12", 3000);
		//webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
		webSocket.onEvent(webSocketEvent);

		//NTP Clint
		timeClient.begin();
		timeClient.update();
	}
	else {
		displayManager.display.println("Conex�o com a rede WiFi falou!");
		displayManager.display.display();
		delay(2000);
 }
}

void printAddressSerial(byte deviceAddress[8])
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

void printDataSerial(TemperatureSensor temperatureSensor)
{
	Serial.print("Address: ");
	printAddressSerial(temperatureSensor.deviceAddress);
	Serial.print(" ValidFamily: ");
	Serial.print(temperatureSensor.validFamily);
	Serial.print(" Family: ");
	Serial.print(temperatureSensor.family);
	Serial.print(" Connected: ");
	Serial.print(temperatureSensor.isConnected);
	Serial.print(" Resolution: ");
	Serial.print(temperatureSensor.resolution);
	Serial.print(" Temp C: ");
	Serial.print(temperatureSensor.tempCelsius);
	Serial.print(" Temp F: ");
	Serial.print(temperatureSensor.tempFahrenheit);
	Serial.print(" HasAlarm: ");
	Serial.print(temperatureSensor.hasAlarm);
	Serial.print(" LowAlarmTemp: ");
	Serial.print(temperatureSensor.lowAlarmTemp);
	Serial.print(" HighAlarmTemp: ");
	Serial.print(temperatureSensor.highAlarmTemp);
	Serial.println();
}

void printAddressDisplay(byte deviceAddress[8])
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) displayManager.display.print("0");
		displayManager.display.print(deviceAddress[i], HEX);
	}
}

void printDataDisplay(TemperatureSensor temperatureSensor)
{
	displayManager.display.print("Address=");
	printAddressDisplay(temperatureSensor.deviceAddress);
	displayManager.display.println();
	displayManager.display.setTextSize(2);
	displayManager.display.print(temperatureSensor.tempCelsius);
	displayManager.display.println(" C ");
	displayManager.display.print(temperatureSensor.tempFahrenheit);
	displayManager.display.println(" F");
}

void loop() {	

	webSocket.loop();

	if (isConnected) {

		// text display tests
		displayManager.display.clearDisplay();
		displayManager.display.setTextSize(1);
		displayManager.display.setTextColor(WHITE);
		displayManager.display.setCursor(0, 0);

		uint64_t now = millis();

		if (now - messageTimestamp > MESSAGE_INTERVAL) {

			messageTimestamp = now;

			TemperatureSensor *arr = temperatureSensorManager.getSensors();

			String json = "";
			for (int i = 0; i < sizeof(arr) / sizeof(int); ++i) {
				json += arr[i].json;
				printDataSerial(arr[i]);
				printDataDisplay(arr[i]);
			}
			json += "";

			String data = "42[\"sendTemp\"," + json + "]";
			webSocket.sendTXT(data);

		}
		if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
			heartbeatTimestamp = now;
			// socket.io heartbeat message
			webSocket.sendTXT("2");
		}
	}
}