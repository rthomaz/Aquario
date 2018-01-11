#ifndef SensorInDevice_h
#define SensorInDevice_h

#include "ArduinoJson.h"
#include "Sensor.h"

namespace ART
{
	class DeviceSensors;

	class SensorInDevice
	{

	public:
		SensorInDevice(DeviceSensors* deviceSensors, JsonObject& jsonObject);
		~SensorInDevice();

		short								getOrdination();
		void								setOrdination(short value);

		static SensorInDevice create(DeviceSensors* deviceSensors, JsonObject& jsonObject)
		{
			return SensorInDevice(deviceSensors, jsonObject);
		}

	private:

		DeviceSensors *						_deviceSensors;
		Sensor *							_sensor;

		short 								_ordination;
	};
}

#endif