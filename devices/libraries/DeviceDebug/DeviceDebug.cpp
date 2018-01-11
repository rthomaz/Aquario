#include "DeviceDebug.h"
#include "ESPDevice.h"

namespace ART
{
	DeviceDebug::DeviceDebug(ESPDevice* espDevice)
	{
		_espDevice = espDevice;
		_debug = new RemoteDebug();
	}

	DeviceDebug::~DeviceDebug()
	{
		delete (_espDevice);
		delete (_debug);
	}

	void DeviceDebug::loop()
	{
		_debug->handle();
	}

	void DeviceDebug::load(JsonObject& jsonObject)
	{
		JsonObject& deviceDebugJO = jsonObject["deviceDebug"];
		JsonObject& deviceWiFiJO = jsonObject["deviceWiFi"];

		setHostName(strdup(deviceWiFiJO["hostName"]));
		setRemoteEnabled(deviceDebugJO["remoteEnabled"].as<bool>());
		setSerialEnabled(deviceDebugJO["serialEnabled"].as<bool>());
		setResetCmdEnabled(deviceDebugJO["resetCmdEnabled"].as<bool>());
		setShowColors(deviceDebugJO["showColors"].as<bool>());
		setShowDebugLevel(deviceDebugJO["showDebugLevel"].as<bool>());
		setShowProfiler(deviceDebugJO["showProfiler"].as<bool>());
		setShowTime(deviceDebugJO["showTime"].as<bool>());
	}

	bool DeviceDebug::isActive(uint8_t debugLevel)
	{
		return _debug->isActive(debugLevel);
	}

	int DeviceDebug::print(const char* className, const char* caller, const char* message)
	{
		return _debug->printf(createExpression(className, caller, message).c_str());
	}

	template<typename... Args>
	int DeviceDebug::printf(const char* className, const char* caller, const char* format, Args... args)
	{
		return _debug->printf(createExpression(className, caller, format).c_str(), args...);
	}

	std::string DeviceDebug::createExpression(const char* className, const char* caller, const char* expression)
	{
		std::string str;
		str.append(className);
		str.append(" ");
		str.append(caller);
		str.append(" ");
		str.append(expression);
		return str;
	}

	void DeviceDebug::setRemoteEnabled(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setRemoteEnabled", "Parse failed: %s\n", json);
			return;
		}
		setRemoteEnabled(root["value"].as<bool>());
	}

	void DeviceDebug::setResetCmdEnabled(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setResetCmdEnabled", "Parse failed: %s\n", json);
			return;
		}
		setResetCmdEnabled(root["value"].as<bool>());
	}

	void DeviceDebug::setSerialEnabled(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setSerialEnabled", "Parse failed: %s\n", json);
			return;
		}
		setSerialEnabled(root["value"].as<bool>());
	}

	void DeviceDebug::setShowColors(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setShowColors", "Parse failed: %s\n", json);
			return;
		}
		setShowColors(root["value"].as<bool>());
	}

	void DeviceDebug::setShowDebugLevel(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setShowDebugLevel", "Parse failed: %s\n", json);
			return;
		}
		setShowDebugLevel(root["value"].as<bool>());
	}

	void DeviceDebug::setShowProfiler(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setShowProfiler", "Parse failed: %s\n", json);
			return;
		}
		setShowProfiler(root["value"].as<bool>());
	}

	void DeviceDebug::setShowTime(char* json)
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		if (!root.success()) {
			printf("DeviceDebug", "setShowTime", "Parse failed: %s\n", json);
			return;
		}
		setShowTime(root["value"].as<bool>());
	}

	void DeviceDebug::initTelnetServer()
	{
		if (!_telnetServer) {
			_telnetServer = true;
			MDNS.addService("telnet", "tcp", TELNET_PORT);
		}
	}

	void DeviceDebug::setHostName(char* value)
	{
		_hostName = new char(sizeof(strlen(value)));
		_hostName = value;
		printf("DeviceDebug", "setHostName", "hostName: %s\n", _hostName);
	}

	void DeviceDebug::setRemoteEnabled(bool value)
	{
		_remoteEnabled = value;
		if (_remoteEnabled) {
			initTelnetServer();
			_debug->begin(_hostName);
		}
		else {
			_debug->stop();
		}
		printf("DeviceDebug", "setRemoteEnabled", "remoteEnabled: %s\n", _remoteEnabled ? "true" : "false");
	}

	void DeviceDebug::setResetCmdEnabled(bool value)
	{
		_resetCmdEnabled = value;
		_debug->setResetCmdEnabled(_resetCmdEnabled);
		printf("DeviceDebug", "setResetCmdEnabled", "resetCmdEnabled: %s\n", _resetCmdEnabled ? "true" : "false");
	}

	void DeviceDebug::setSerialEnabled(bool value)
	{
		_serialEnabled = value;
		_debug->setSerialEnabled(_serialEnabled);
		printf("DeviceDebug", "setSerialEnabled", "serialEnabled: %s\n", _serialEnabled ? "true" : "false");
	}

	void DeviceDebug::setShowColors(bool value)
	{
		_showColors = value;
		_debug->showColors(_showColors);
		printf("DeviceDebug", "setShowColors", "showColors: %s\n", _showColors ? "true" : "false");
	}

	void DeviceDebug::setShowDebugLevel(bool value)
	{
		_showDebugLevel = value;
		_debug->showDebugLevel(_showDebugLevel);
		printf("DeviceDebug", "setShowDebugLevel", "showDebugLevel: %s\n", _showDebugLevel ? "true" : "false");
	}

	void DeviceDebug::setShowProfiler(bool value)
	{
		_showProfiler = value;
		_debug->showProfiler(_showProfiler);
		printf("DeviceDebug", "setShowProfiler", "showProfiler: %s\n", _showProfiler ? "true" : "false");
	}

	void DeviceDebug::setShowTime(bool value)
	{
		_showTime = value;
		_debug->showTime(_showTime);
		printf("DeviceDebug", "setShowTime", "showTime: %s\n", _showTime ? "true" : "false");
	}
}