﻿'use strict';
app.factory('unitOfMeasurementService', ['$http', 'ngAuthSettings', '$rootScope', 'stompService', function ($http, ngAuthSettings, $rootScope, stompService) {

    var serviceBase = ngAuthSettings.distributedServicesUri;

    var _initializing = false;
    var _initialized  = false;

    var serviceFactory = {};    

    var onConnected = function () {

        stompService.subscribe('UnitOfMeasurement.GetAllViewCompleted', onGetAllCompleted);

        if (!_initializing && !_initialized) {
            _initializing = true;
            getAll();
        }
    }   

    var initialized = function () {
        return _initialized;
    };

    var getAll = function () {
        return $http.post(serviceBase + 'api/unitOfMeasurement/getAll').then(function (results) {
            //alert('envio bem sucedido');
        });
    };     

    var getScaleById = function (unitOfMeasurementId) {
        for (var i = 0; i < serviceFactory.scales.length; i++) {
            if (serviceFactory.scales[i].id === unitOfMeasurementId) {
                return serviceFactory.scales[i];
            }
        }
    };

    var onGetAllCompleted = function (payload) {
        var dataUTF8 = decodeURIComponent(escape(payload.body));
        var data = JSON.parse(dataUTF8);
        for (var i = 0; i < data.length; i++) {
            serviceFactory.scales.push(data[i]);
        }
        _initializing = false;
        _initialized = true;
        $rootScope.$emit('UnitOfMeasurementService_Initialized');
    }

    $rootScope.$on('$destroy', function () {
        clearOnConnected();
    });

    var clearOnConnected = $rootScope.$on('stompService_onConnected', onConnected);        

    // stompService
    if (stompService.connected())
        onConnected();

    // serviceFactory
        
    serviceFactory.scales = [];  

    serviceFactory.initialized = initialized;
    serviceFactory.getScaleById = getScaleById;    

    return serviceFactory;

}]);