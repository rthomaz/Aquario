﻿'use strict';
app.factory('deviceMapper', ['$rootScope', 'deviceContext', 'deviceConstant', 'timeZoneConstant', function ($rootScope, deviceContext, deviceConstant, timeZoneConstant) {

    var serviceFactory = {};    

    // *** Navigation Properties Mappers ***

    var mapper_DeviceNTP_TimeZone_Init = false;
    //var mapper_DeviceNTP_TimeZone = function () {
    //    if (!mapper_DeviceNTP_TimeZone_Init && deviceContext.deviceLoaded && deviceContext.timeZoneLoaded) {
    //        mapper_DeviceNTP_TimeZone_Init = true;
    //        for (var i = 0; i < deviceContext.deviceNTP.length; i++) {
    //            applyTimeZoneInDeviceNTP(deviceContext.deviceNTP[i]);
    //        }
    //    }
    //};   

    var applyTimeZoneInDeviceNTP = function (deviceNTP) {
        var timeZone = deviceContext.getTimeZoneByKey(deviceNTP.timeZoneId);
        deviceNTP.timeZone = timeZone;
        delete deviceNTP.timeZoneId; // removendo a foreing key
    }

    // *** Events Subscriptions

    var onDeviceGetAllByApplicationIdCompleted = function (event, data) {
        deviceGetAllByApplicationIdCompletedSubscription();
        deviceContext.deviceLoaded = true;
    }      

    var onTimeZoneGetAllCompleted = function (event, data) {
        timeZoneGetAllCompletedSubscription();
    }  

    var deviceGetAllByApplicationIdCompletedSubscription = $rootScope.$on(deviceConstant.getAllByApplicationIdCompletedEventName, onDeviceGetAllByApplicationIdCompleted);
    var timeZoneGetAllCompletedSubscription = $rootScope.$on(timeZoneConstant.getAllCompletedEventName, onTimeZoneGetAllCompleted);
        
    $rootScope.$on('$destroy', function () {
        deviceGetAllByApplicationIdCompletedSubscription();  
        timeZoneGetAllCompletedSubscription();
    });

    // *** Events Subscriptions











    // *** Watches ***    

    //deviceContext.$watchCollection('device', function (newValues, oldValues) {
    //    for (var i = 0; i < newValues.length; i++) {
    //        var device = newValues[i];
    //        deviceContext.deviceNTP.device = device;
    //        deviceContext.deviceNTP.push(device.deviceNTP);
    //        deviceContext.deviceSensors.device = device;            
    //        deviceContext.deviceSensors.push(device.deviceSensors);
    //    }
    //});

    //deviceContext.$watchCollection('deviceNTP', function (newValues, oldValues) {
    //    for (var i = 0; i < newValues.length; i++) {
    //        var deviceNTP = newValues[i];
    //        if (deviceContext.timeZoneLoaded) {
    //            applyTimeZoneInDeviceNTP(deviceNTP);
    //        }
    //    }
    //});

    //deviceContext.$watchCollection('deviceSensors', function (newValues, oldValues) {
    //    for (var i = 0; i < newValues.length; i++) {
    //        var deviceSensors = newValues[i];
    //        deviceContext.deviceSensors.push(deviceSensors);
    //    }
    //});

    //deviceContext.$watchCollection('sensorsInDevice', function (newValues, oldValues) {
    //    for (var i = 0; i < newValues.length; i++) {
    //        var sensorsInDevice = newValues[i];
    //    }
    //});

    deviceContext.$watch('deviceLoaded', function (newValue, oldValue) {
        //mapper_DeviceNTP_TimeZone();
    });

    deviceContext.$watch('timeZoneLoaded', function (newValue, oldValue) {
        //mapper_DeviceNTP_TimeZone();
    });
    
    return serviceFactory;

}]);