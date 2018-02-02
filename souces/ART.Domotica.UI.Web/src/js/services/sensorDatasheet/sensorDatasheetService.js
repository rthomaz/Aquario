﻿'use strict';
app.factory('sensorDatasheetService', ['$http', 'ngAuthSettings', '$rootScope', '$localStorage', 'stompService', 'sensorDatasheetContext', 'sensorDatasheetConstant',
    function ($http, ngAuthSettings, $rootScope, $localStorage, stompService, sensorDatasheetContext, sensorDatasheetConstant) {

        var serviceFactory = {};

        // Local cache        

        if ($localStorage.sensorDatasheetData) {
            var data = JSON.parse(Base64.decode($localStorage.sensorDatasheetData));
            for (var i = 0; i < data.length; i++) {
                sensorDatasheetContext.sensorDatasheet.push(data[i]);
            }
            $rootScope.$emit(sensorDatasheetConstant.getAllCompletedEventName);
            return serviceFactory;
        }

        // Get from Server

        var _initializing = false;
        var _initialized = false;

        var serviceBase = ngAuthSettings.distributedServicesUri;

        var getAllCompletedSubscription = null;

        var onConnected = function () {

            getAllCompletedSubscription = stompService.subscribeView(sensorDatasheetConstant.getAllCompletedTopic, onGetAllCompleted);

            if (!_initializing && !_initialized) {
                _initializing = true;
                getAll();
            }
        }

        var getAll = function () {
            return $http.post(serviceBase + sensorDatasheetConstant.getAllApiUri).then(function (results) {
                //alert('envio bem sucedido');
            });
        };

        var onGetAllCompleted = function (payload) {

            var dataUTF8 = decodeURIComponent(escape(payload.body));

            $localStorage.sensorDatasheetData = Base64.encode(dataUTF8);
            $localStorage.$save();

            var data = JSON.parse(dataUTF8);

            for (var i = 0; i < data.length; i++) {
                sensorDatasheetContext.sensorDatasheet.push(data[i]);
            }

            sensorDatasheetContext.$digest();

            _initializing = false;
            _initialized = true;

            clearOnConnected();

            getAllCompletedSubscription.unsubscribe();

            $rootScope.$emit(sensorDatasheetConstant.getAllCompletedEventName);
        }

        $rootScope.$on('$destroy', function () {
            clearOnConnected();
        });

        // stompService

        var clearOnConnected = $rootScope.$on(stompService.connectedEventName, onConnected);

        if (stompService.connected()) onConnected();

        return serviceFactory;

    }]);