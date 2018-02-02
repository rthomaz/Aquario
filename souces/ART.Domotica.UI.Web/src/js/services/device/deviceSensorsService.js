﻿'use strict';
app.factory('deviceSensorsService', ['$http', 'ngAuthSettings', '$rootScope', 'stompService', 'deviceContext', 'deviceSensorsConstant', 'deviceSensorsFinder',
    function ($http, ngAuthSettings, $rootScope, stompService, deviceContext, deviceSensorsConstant, deviceSensorsFinder) {

        var serviceFactory = {};

        var serviceBase = ngAuthSettings.distributedServicesUri;

        var messageIoTReceivedSubscription = null;
        var setReadIntervalInMilliSecondsCompletedSubscription = null;
        var setPublishIntervalInMilliSecondsCompletedSubscription = null;

        var setReadIntervalInMilliSeconds = function (deviceSensorsId, deviceDatasheetId, readIntervalInMilliSeconds) {
            var data = {
                deviceId: deviceSensorsId,
                deviceDatasheetId: deviceDatasheetId,
                intervalInMilliSeconds: readIntervalInMilliSeconds,
            }
            return $http.post(serviceBase + deviceSensorsConstant.setReadIntervalInMilliSecondsApiUri, data).then(function (results) {
                return results;
            });
        };

        var setPublishIntervalInMilliSeconds = function (deviceSensorsId, deviceDatasheetId, publishIntervalInMilliSeconds) {
            var data = {
                deviceId: deviceSensorsId,
                deviceDatasheetId: deviceDatasheetId,
                intervalInMilliSeconds: publishIntervalInMilliSeconds,
            }
            return $http.post(serviceBase + deviceSensorsConstant.setPublishIntervalInMilliSecondsApiUri, data).then(function (results) {
                return results;
            });
        };

        var onConnected = function () {
            messageIoTReceivedSubscription = stompService.subscribeDevice(deviceSensorsConstant.messageIoTTopic, onMessageIoTReceived);            
            setReadIntervalInMilliSecondsCompletedSubscription = stompService.subscribeAllViews(deviceSensorsConstant.setReadIntervalInMilliSecondsCompletedTopic, onSetReadIntervalInMilliSecondsCompleted);
            setPublishIntervalInMilliSecondsCompletedSubscription = stompService.subscribeAllViews(deviceSensorsConstant.setPublishIntervalInMilliSecondsCompletedTopic, onSetPublishIntervalInMilliSecondsCompleted);
        }

        var onMessageIoTReceived = function (payload) {
            var dataUTF8 = decodeURIComponent(escape(payload.body));
            var data = JSON.parse(dataUTF8);
            var deviceSensors = deviceSensorsFinder.getByKey(data.deviceId, data.deviceDatasheetId);
            if(angular.isUndefined(deviceSensors)) return;
            deviceSensors.epochTimeUtc = data.epochTimeUtc;            
            deviceContext.$digest();
            $rootScope.$emit(deviceSensorsConstant.messageIoTEventName + data.deviceId, data);
        }

        var onSetReadIntervalInMilliSecondsCompleted = function (payload) {
            var result = JSON.parse(payload.body);
            var deviceSensors = deviceSensorsFinder.getByKey(result.deviceSensorsId, result.deviceDatasheetId);
            deviceSensors.readIntervalInMilliSeconds = result.readIntervalInMilliSeconds;
            deviceContext.$digest();
            $rootScope.$emit(deviceSensorsConstant.setReadIntervalInMilliSecondsCompletedEventName + result.deviceSensorsId, result);
        };

        var onSetPublishIntervalInMilliSecondsCompleted = function (payload) {
            var result = JSON.parse(payload.body);
            var deviceSensors = deviceSensorsFinder.getByKey(result.deviceId, result.deviceDatasheetId);
            deviceSensors.publishIntervalInMilliSeconds = result.publishIntervalInMilliSeconds;
            deviceContext.$digest();
            $rootScope.$emit(deviceSensorsConstant.setPublishIntervalInMilliSecondsCompletedEventName + result.deviceId, result);
        };

        $rootScope.$on('$destroy', function () {
            clearOnConnected();
            messageIoTReceivedSubscription.unsubscribe();
            setReadIntervalInMilliSecondsCompletedSubscription.unsubscribe();
            setPublishIntervalInMilliSecondsCompletedSubscription.unsubscribe();
        });

        var clearOnConnected = $rootScope.$on(stompService.connectedEventName, onConnected);

        // stompService
        if (stompService.connected()) onConnected();

        // serviceFactory

        serviceFactory.setReadIntervalInMilliSeconds = setReadIntervalInMilliSeconds;
        serviceFactory.setPublishIntervalInMilliSeconds = setPublishIntervalInMilliSeconds;

        return serviceFactory;

        var updateSensors = function (device, newSensors) {
            var oldSensors = device.sensors;
            for (var i = 0; i < oldSensors.length; i++) {
                for (var j = 0; j < newSensors.length; j++) {
                    if (oldSensors[i].sensorTempDSFamilyId === newSensors[j].sensorTempDSFamilyId) {

                        oldSensors[i].isConnected = newSensors[j].isConnected;

                        //Temp
                        oldSensors[i].tempCelsius = newSensors[j].tempCelsius;
                        //oldSensors[i].tempConverted = unitMeasurementConverter.convertFromCelsius(oldSensors[i].unitMeasurementId, oldSensors[i].tempCelsius);

                        //Chart

                        oldSensors[i].chart[1].key = 'Temperatura ' + oldSensors[i].tempCelsius + ' °C';

                        oldSensors[i].chart[0].values.push({
                            epochTime: device.epochTimeUtc,
                            temperature: oldSensors[i].highAlarm.alarmCelsius,
                        });

                        oldSensors[i].chart[1].values.push({
                            epochTime: device.epochTimeUtc,
                            temperature: oldSensors[i].tempCelsius,
                        });

                        oldSensors[i].chart[2].values.push({
                            epochTime: device.epochTimeUtc,
                            temperature: oldSensors[i].lowAlarm.alarmCelsius,
                        });

                        if (oldSensors[i].chart[0].values.length > 60)
                            oldSensors[i].chart[0].values.shift();

                        if (oldSensors[i].chart[1].values.length > 60)
                            oldSensors[i].chart[1].values.shift();

                        if (oldSensors[i].chart[2].values.length > 60)
                            oldSensors[i].chart[2].values.shift();

                        break;
                    }
                }
            }
        }

        var chartLine = function (key) {
            this.key = key;
            this.values = [];
        }

        var insertDeviceInCollection = function (device) {
            //device.createDate = new Date(device.createDate * 1000).toLocaleString();
            //deviceContext.device.push(device);
            //for (var i = 0; i < device.sensors.length; i++) {

            //var sensor = device.sensors[i];

            //temp
            //sensor.tempConverted = null;

            //unitMeasurement

            // Arrumar aqui !!!
            //sensor.unitMeasurement = siContext.getUnitMeasurementScaleByKey(sensor.unitMeasurementId);

            //sensorUnitMeasurementScale
            //sensor.sensorUnitMeasurementScale.maxConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.sensorUnitMeasurementScale.max);
            //sensor.sensorUnitMeasurementScale.minConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.sensorUnitMeasurementScale.min);

            //alarms
            //sensor.highAlarm.alarmConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.highAlarm.alarmCelsius);
            //sensor.lowAlarm.alarmConverted = unitMeasurementConverter.convertFromCelsius(sensor.unitMeasurementId, sensor.lowAlarm.alarmCelsius);

            //Chart
            //sensor.chart = [];
            //sensor.chart.push(new chartLine("Máximo"));
            //sensor.chart.push(new chartLine("Temperatura"));
            //sensor.chart.push(new chartLine("Mínimo"));
            //}
        }

    }]);