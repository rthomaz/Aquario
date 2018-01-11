#ifndef DisplayTemperatureSensorManager_h
#define DisplayTemperatureSensorManager_h

#include "Arduino.h"
#include "DisplayManager.h"
#include "ESPDevice.h"
#include "UnitOfMeasurementConverter.h"

using namespace ART;

class DisplayTemperatureSensorManager
{

public:
	DisplayTemperatureSensorManager(DisplayManager& displayManager, ESPDevice& espDevice, UnitOfMeasurementConverter& unitOfMeasurementConverter);
	~DisplayTemperatureSensorManager();

	void							printUpdate(bool on);
	void							printSensors();

private:

	DisplayManager * _displayManager;
	ESPDevice*  					_espDevice;
	UnitOfMeasurementConverter*  	_unitOfMeasurementConverter;

	void							printBar(SensorOld& sensor, int x, int y, int width, int height);
	void							printBarValue(SensorOld& sensor, int x, int y, int width, int height);
	void							printText(SensorOld& sensor, int x, int y);
};

#endif