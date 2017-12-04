﻿'use strict';
app.factory('sensorService', ['$http', 'ngAuthSettings', '$rootScope', 'stompService', function ($http, ngAuthSettings, $rootScope, stompService) {

    var serviceBase = ngAuthSettings.distributedServicesUri;

    var _initializing = false;
    var _initialized  = false;

    var serviceFactory = {};    

    var onConnected = function () {

        stompService.subscribe('Sensor.GetAllViewCompleted', onGetAllCompleted);
        stompService.subscribeAllViews('Sensor.SetUnitMeasurementViewCompleted', onSetUnitMeasurementCompleted);
        stompService.subscribeAllViews('Sensor.SetLabelViewCompleted', onSetLabelCompleted);

        if (!_initializing && !_initialized) {
            _initializing = true;
            getAll();
        }
    }   

    var initialized = function () {
        return _initialized;
    };

    var getAll = function () {
        return $http.post(serviceBase + 'api/sensor/getAll').then(function (results) {
            //alert('envio bem sucedido');
        });
    };     

    var setUnitMeasurement = function (sensorTempDSFamilyId, unitMeasurementId) {
        var data = {
            sensorTempDSFamilyId: sensorTempDSFamilyId,
            unitMeasurementId: unitMeasurementId,
        }
        return $http.post(serviceBase + 'api/sensor/setUnitMeasurement', data).then(function (results) {
            return results;
        });
    };

    var setLabel = function (sensorTempDSFamilyId, label) {
        var data = {
            sensorTempDSFamilyId: sensorTempDSFamilyId,
            label: label,
        }
        return $http.post(serviceBase + 'api/sensor/setLabel', data).then(function (results) {
            return results;
        });
    };  

    var getSensorById = function (sensorId) {
        for (var i = 0; i < serviceFactory.sensors.length; i++) {
            if (serviceFactory.sensors[i].id === sensorId) {
                return serviceFactory.sensors[i];
            }
        }
    };

    var onGetAllCompleted = function (payload) {
        var dataUTF8 = decodeURIComponent(escape(payload.body));
        var data = JSON.parse(dataUTF8);
        for (var i = 0; i < data.length; i++) {
            serviceFactory.sensors.push(data[i]);
        }
        _initializing = false;
        _initialized = true;
        $rootScope.$emit('sensorService_Initialized');
    }

    var onSetUnitMeasurementCompleted = function (payload) {

        var result = JSON.parse(payload.body);
        var sensor = getById(result.deviceId, result.sensorTempDSFamilyId);

        //unitMeasurement
        sensor.unitMeasurementId = result.unitMeasurementId;
        sensor.unitMeasurement = unitMeasurementService.getByKey(sensor.unitMeasurementId);

        //temp
        sensor.tempConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.tempCelsius);

        //sensorUnitMeasurementScale
        sensor.sensorUnitMeasurementScale.maxConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.sensorUnitMeasurementScale.max);
        sensor.sensorUnitMeasurementScale.minConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.sensorUnitMeasurementScale.min);

        //alarms
        sensor.highAlarm.alarmConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.highAlarm.alarmCelsius);
        sensor.lowAlarm.alarmConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.lowAlarm.alarmCelsius);

        $rootScope.$emit('sensorService_onSetUnitMeasurementCompleted_Id_' + result.sensorTempDSFamilyId, result);
    }    

    var onSetLabelCompleted = function (payload) {
        var result = JSON.parse(payload.body);
        var sensor = getById(result.deviceId, result.sensorTempDSFamilyId);
        sensor.label = result.label;
        $rootScope.$emit('service_onSetLabelCompleted_Id_' + result.sensorTempDSFamilyId, result);
    }   

    $rootScope.$on('$destroy', function () {
        clearOnConnected();
    });

    var clearOnConnected = $rootScope.$on(stompService.connectedEventName, onConnected);       

    // stompService
    if (stompService.connected()) onConnected();

    // serviceFactory
        
    serviceFactory.sensors = [];  

    serviceFactory.initialized = initialized;
    serviceFactory.getSensorById = getSensorById;    
    serviceFactory.setUnitMeasurement = setUnitMeasurement;
    serviceFactory.setLabel = setLabel;   

    return serviceFactory;

}]);