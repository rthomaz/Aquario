﻿'use strict';
app.controller('espDeviceListController', ['$scope', '$timeout', '$log', 'EventDispatcher', 'espDeviceService', function ($scope, $timeout, $log, EventDispatcher, espDeviceService) {    
   
    $scope.devices = espDeviceService.devices;    

}]);

app.controller('espDeviceItemController', ['$scope', '$timeout', '$log', 'EventDispatcher', 'espDeviceService', function ($scope, $timeout, $log, EventDispatcher, espDeviceService) {

    $scope.device = {};

    $scope.init = function (device) {
        $scope.device = device;        
    }

}]);

app.controller('dsFamilyTempSensorItemController', ['$scope', '$rootScope', '$timeout', '$log', 'toaster', 'espDeviceService', 'dsFamilyTempSensorService', 'temperatureScaleService', function ($scope, $rootScope, $timeout, $log, toaster, espDeviceService, dsFamilyTempSensorService, temperatureScaleService) {

    $scope.sensor = {};           

    $scope.selectedHasAlarm = false;
    $scope.selectedLowAlarm = -55;
    $scope.selectedHighAlarm = 125;

    $scope.scale = {
        availableScales: temperatureScaleService.scales,
        selectedScale: {},
    };

    $scope.resolution = {
        availableResolutions: dsFamilyTempSensorService.resolutions,
        selectedResolution: {},
    };

    $scope.changeScale = function () {
        dsFamilyTempSensorService.setScale($scope.sensor.dsFamilyTempSensorId, $scope.scale.selectedScale.id);
    };

    $scope.changeResolution = function () {
        dsFamilyTempSensorService.setResolution($scope.sensor.dsFamilyTempSensorId, $scope.resolution.selectedResolution.id);
    };    

    $scope.changeHasAlarm = function () {
        if ($scope.selectedHasAlarm)
            dsFamilyTempSensorService.setAlarmOn($scope.sensor.dsFamilyTempSensorId, $scope.selectedLowAlarm, $scope.selectedHighAlarm);
        else
            dsFamilyTempSensorService.setAlarmOff($scope.sensor.dsFamilyTempSensorId);
    };

    $scope.changeLowAlarm = function () {
        dsFamilyTempSensorService.setLowAlarm($scope.sensor.dsFamilyTempSensorId, $scope.selectedLowAlarm);
    };

    $scope.changeHighAlarm = function () {
        dsFamilyTempSensorService.setHighAlarm($scope.sensor.dsFamilyTempSensorId, $scope.selectedHighAlarm);
    };

    $scope.init = function (sensor) {

        $scope.sensor = sensor;
        
        $scope.scale.selectedScale = temperatureScaleService.getScaleById(sensor.temperatureScaleId);
        $scope.resolution.selectedResolution = dsFamilyTempSensorService.getResolutionById(sensor.dsFamilyTempSensorResolutionId);

        $scope.selectedHasAlarm = sensor.hasAlarm;
        $scope.selectedLowAlarm = sensor.lowAlarm;
        $scope.selectedHighAlarm = sensor.highAlarm;

        clearOnSetScaleCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetScaleCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetScaleCompleted);
        clearOnSetResolutionCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetResolutionCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetResolutionCompleted);
        clearOnSetAlarmOnCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetAlarmOnCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetAlarmOnCompleted);
        clearOnSetAlarmOffCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetAlarmOffCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetAlarmOffCompleted);
        clearOnSetLowAlarmCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetLowAlarmCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetLowAlarmCompleted);
        clearOnSetHighAlarmCompleted = $rootScope.$on('dsFamilyTempSensorService_onSetHighAlarmCompleted_Id_' + $scope.sensor.dsFamilyTempSensorId, onSetHighAlarmCompleted);
    };    

    var clearOnSetScaleCompleted = null;
    var clearOnSetResolutionCompleted = null;
    var clearOnSetAlarmOnCompleted = null;
    var clearOnSetAlarmOffCompleted = null;
    var clearOnSetLowAlarmCompleted = null;
    var clearOnSetHighAlarmCompleted = null;
    
    $scope.$on('$destroy', function () {
        clearOnSetScaleCompleted();
        clearOnSetResolutionCompleted();
        clearOnSetAlarmOnCompleted();
        clearOnSetAlarmOffCompleted();
        clearOnSetLowAlarmCompleted();
        clearOnSetHighAlarmCompleted();
    });

    var onSetScaleCompleted = function (event, data) {
        $scope.sensor.temperatureScaleId = data.temperatureScaleId;
        toaster.pop('success', 'Sucesso', 'escala alterada');
    };

    var onSetResolutionCompleted = function (event, data) {
        $scope.sensor.dsFamilyTempSensorResolutionId = data.dsFamilyTempSensorResolutionId;
        toaster.pop('success', 'Sucesso', 'resolução alterada');
    };

    var onSetAlarmOnCompleted = function (event, data) {
        toaster.pop('success', 'Sucesso', 'alarme ligado');
    };

    var onSetAlarmOffCompleted = function (event, data) {
        toaster.pop('success', 'Sucesso', 'alarme desligado');
    };

    var onSetLowAlarmCompleted = function (event, data) {
        toaster.pop('success', 'Sucesso', 'alarme baixo alterado');
    };

    var onSetHighAlarmCompleted = function (event, data) {
        toaster.pop('success', 'Sucesso', 'alarme alto alterado');
    }; 

}]);