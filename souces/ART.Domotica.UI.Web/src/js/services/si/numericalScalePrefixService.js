﻿'use strict';
app.factory('numericalScalePrefixService', ['$http', 'ngAuthSettings', '$rootScope', 'stompService', 'contextScope', function ($http, ngAuthSettings, $rootScope, stompService, contextScope) {

    var serviceBase = ngAuthSettings.distributedServicesUri;

    var _initializing = false;
    var _initialized  = false;

    var serviceFactory = {};    

    var onConnected = function () {

        stompService.subscribe('SI.NumericalScalePrefix.GetAllViewCompleted', onGetAllCompleted);

        if (!_initializing && !_initialized) {
            _initializing = true;
            getAll();
        }
    }   

    var initialized = function () {
        return _initialized;
    };

    var getAll = function () {
        return $http.post(serviceBase + 'api/si/numericalScalePrefix/getAll').then(function (results) {
            //alert('envio bem sucedido');
        });
    };       

    var onGetAllCompleted = function (payload) {
        var dataUTF8 = decodeURIComponent(escape(payload.body));
        var data = JSON.parse(dataUTF8);
        for (var i = 0; i < data.length; i++) {
            contextScope.numericalScalePrefixs.push(data[i]);
        }
        contextScope.numericalScalePrefixLoaded = true;
        _initializing = false;
        _initialized = true;
        $rootScope.$emit('numericalScalePrefixService_Initialized');
    }

    $rootScope.$on('$destroy', function () {
        clearOnConnected();
    });

    var clearOnConnected = $rootScope.$on('stompService_onConnected', onConnected);        

    // stompService
    if (stompService.connected())
        onConnected();

    // serviceFactory

    serviceFactory.initialized = initialized;

    return serviceFactory;

}]);