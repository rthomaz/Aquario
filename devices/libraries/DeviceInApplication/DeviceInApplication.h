#ifndef DeviceInApplication_h
#define DeviceInApplication_h

#include "functional"
#include "vector"
#include "Arduino.h"
#include "ArduinoJson.h"

#define DEVICE_IN_APPLICATION_INSERT_TOPIC_SUB "DeviceInApplication/InsertIoT"
#define DEVICE_IN_APPLICATION_REMOVE_TOPIC_SUB "DeviceInApplication/RemoveIoT"

namespace ART
{
	class ESPDevice;

	class DeviceInApplication
	{

	public:

		DeviceInApplication(ESPDevice* espDevice);
		~DeviceInApplication();		

		static void							create(DeviceInApplication* (&deviceInApplication), ESPDevice* espDevice);

		void								begin();

		void								load(JsonObject& jsonObject);

		void								insert(char* json);
		void								remove();

		char*								getApplicationId() const;		
		char*								getApplicationTopic()  const;		

		bool								inApplication();

		typedef std::function<void()>		callbackSignature;

		void								setInsertCallback(callbackSignature callback);
		void								setRemoveCallback(callbackSignature callback);

	private:

		ESPDevice *							_espDevice;

		char*								_applicationId;
		char*								_applicationTopic;		

		callbackSignature					_insertCallback;
		callbackSignature					_removeCallback;

		void								setApplicationId(const char* value);
		void								setApplicationTopic(const char* value);

		void								onDeviceMQSubscribeDevice();
		void								onDeviceMQUnSubscribeDevice();
		void								onDeviceMQSubscribeDeviceInApplication();		
		void								onDeviceMQUnSubscribeDeviceInApplication();
		bool								onDeviceMQSubscription(char* topicKey, char* json);

	};
}

#endif