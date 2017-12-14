﻿app.controller('sensorUnitMeasurementScaleController', ['$scope', '$rootScope', '$timeout', '$log', 'toaster', 'debounce', 'unitMeasurementConverter', 'sensorUnitMeasurementScaleService', 'sensorDatasheetContext', 'localeContext', 'sensorDatasheetUnitMeasurementScaleFinder', 'unitMeasurementFinder', 'unitMeasurementScaleFinder', 'countryFinder', 'numericalScaleTypeFinder', 'sensorUnitMeasurementScaleConstant',
    function ($scope, $rootScope, $timeout, $log, toaster, debounce, unitMeasurementConverter, sensorUnitMeasurementScaleService, sensorDatasheetContext, localeContext, sensorDatasheetUnitMeasurementScaleFinder, unitMeasurementFinder, unitMeasurementScaleFinder, countryFinder, numericalScaleTypeFinder, sensorUnitMeasurementScaleConstant) {

        $scope.sensorUnitMeasurementScale = null;

        $scope.$watch('sensorUnitMeasurementScale', function (newValue) {
            if (newValue) {                

                applySelectsWithContext();

                initializeSelectedWatches();

                clearOnSetUnitMeasurementNumericalScaleTypeCountryCompleted = $rootScope.$on(sensorUnitMeasurementScaleConstant.setUnitMeasurementNumericalScaleTypeCountryCompletedEventName + newValue.sensorUnitMeasurementScaleId, onSetUnitMeasurementNumericalScaleTypeCountryCompleted);
                //clearOnSetValueCompleted = $rootScope.$on('sensorUnitMeasurementScaleService_SetValueCompleted_Id_' + sensor.sensorUnitMeasurementScale.id, onSetValueCompleted);
            }
        });        

        $scope.init = function (sensorUnitMeasurementScale) {
            $scope.sensorUnitMeasurementScale = sensorUnitMeasurementScale;
        };

        $scope.unitMeasurementView = {
            availables: [],
            selected: null,
        };

        $scope.countryView = {
            availables: [],
            selected: null,
        };

        $scope.numericalScaleTypeView = {
            availables: [],
            selected: null,
        };

        $scope.unitMeasurementScaleView = {
            availables: [],
            selected: null,
        };

        var unitMeasurementViewSelectedWatch = null;
        var countryViewSelectedWatch = null;
        var numericalScaleTypeViewSelectedWatch = null;
        var unitMeasurementScaleViewSelectedWatch = null;

        var initializeSelectedWatches = function () {            

            unitMeasurementViewSelectedWatch = $scope.$watch('unitMeasurementView.selected', function (newValue, oldValue) {
                if (newValue === oldValue) return;
                applyUnitMeasurementScaleView();
                changeUnitMeasurementScale();
            });

            countryViewSelectedWatch = $scope.$watch('countryView.selected', function (newValue, oldValue) {
                if (newValue === oldValue) return;
                applyNumericalScaleTypeView();
                changeUnitMeasurementScale();
            });

            numericalScaleTypeViewSelectedWatch = $scope.$watch('numericalScaleTypeView.selected', function (newValue, oldValue) {
                if (newValue === oldValue) return;
                applyUnitMeasurementScaleView();
                changeUnitMeasurementScale();
            });

            unitMeasurementScaleViewSelectedWatch = $scope.$watch('unitMeasurementScaleView.selected', function (newValue, oldValue) {
                if (newValue === oldValue) return;
                changeUnitMeasurementScale();
            });

        };

        var finalizeSelectedWatches = function () {
            if (unitMeasurementViewSelectedWatch) unitMeasurementViewSelectedWatch();
            if (countryViewSelectedWatch) countryViewSelectedWatch();
            if (numericalScaleTypeViewSelectedWatch) numericalScaleTypeViewSelectedWatch();
            if (unitMeasurementScaleViewSelectedWatch) unitMeasurementScaleViewSelectedWatch();
        };

        var applySelectsWithContext = function () {

            var sensorUnitMeasurementScale = $scope.sensorUnitMeasurementScale;

            //unitMeasurement
            $scope.unitMeasurementView.availables = sensorDatasheetUnitMeasurementScaleFinder.getUnitMeasurementsBySensorDatasheetKey(sensorUnitMeasurementScale.sensorDatasheetId, sensorUnitMeasurementScale.sensorTypeId);
            $scope.unitMeasurementView.selected = unitMeasurementFinder.getByKey(sensorUnitMeasurementScale.unitMeasurementId, sensorUnitMeasurementScale.unitMeasurementTypeId);

            //country
            var country = countryFinder.getByKey(sensorUnitMeasurementScale.countryId);
            $scope.countryView.availables = localeContext.country;
            $scope.countryView.selected = country;

            //numericalScaleType
            $scope.numericalScaleTypeView.availables = sensorDatasheetUnitMeasurementScaleFinder.getNumericalScaleTypesBySensorDatasheetCountryKey(sensorUnitMeasurementScale.sensorDatasheetId, sensorUnitMeasurementScale.sensorTypeId, sensorUnitMeasurementScale.countryId);
            $scope.numericalScaleTypeView.selected = numericalScaleTypeFinder.getByKey(sensorUnitMeasurementScale.numericalScaleTypeId);

            //unitMeasurementScale
            var unitMeasurementScale = unitMeasurementScaleFinder.getByKey(sensorUnitMeasurementScale.unitMeasurementId, sensorUnitMeasurementScale.unitMeasurementTypeId, sensorUnitMeasurementScale.numericalScalePrefixId, sensorUnitMeasurementScale.numericalScaleTypeId);
            $scope.unitMeasurementScaleView.availables = unitMeasurementScaleFinder.getUnitMeasurementScalePrefixes(sensorUnitMeasurementScale.unitMeasurementId, sensorUnitMeasurementScale.unitMeasurementTypeId, sensorUnitMeasurementScale.numericalScaleTypeId);
            $scope.unitMeasurementScaleView.selected = unitMeasurementScale;
        }

        var applyNumericalScaleTypeView = function () {
                        
            var selectedNumericalScaleType = null;

            if ($scope.countryView.selected) {
                var country = $scope.countryView.selected;

                $scope.numericalScaleTypeView.availables = sensorDatasheetUnitMeasurementScaleFinder.getNumericalScaleTypesBySensorDatasheetCountryKey($scope.sensorUnitMeasurementScale.sensorDatasheetId, $scope.sensorUnitMeasurementScale.sensorTypeId, country.countryId);
                                                
                if ($scope.numericalScaleTypeView.availables.length == 1) {
                    selectedNumericalScaleType = $scope.numericalScaleTypeView.availables[0];
                }
            }

            $scope.numericalScaleTypeView.selected = selectedNumericalScaleType;

            var selectNumericalScaleType = $scope.form.selectNumericalScaleType;

            selectNumericalScaleType.$setViewValue(selectedNumericalScaleType);
            selectNumericalScaleType.$commitViewValue();
            selectNumericalScaleType.$render();

        };

        var applyUnitMeasurementScaleView = function () {

            var selectedUnitMeasurementScale = null;

            if ($scope.numericalScaleTypeView.selected && $scope.unitMeasurementView.selected) {
                var numericalScaleType = $scope.numericalScaleTypeView.selected;
                var unitMeasurement = $scope.unitMeasurementView.selected;
                var unitMeasurementScales = unitMeasurementScaleFinder.getUnitMeasurementScalePrefixes(unitMeasurement.unitMeasurementId, unitMeasurement.unitMeasurementTypeId, numericalScaleType.numericalScaleTypeId);
                $scope.unitMeasurementScaleView.availables = unitMeasurementScales;
                if ($scope.unitMeasurementScaleView.availables.length == 1) {
                    selectedUnitMeasurementScale = $scope.unitMeasurementScaleView.availables[0];
                }
            }
            else {
                $scope.unitMeasurementScaleView.availables = [];
            }

            $scope.unitMeasurementScaleView.selected = selectedUnitMeasurementScale;

            var selectUnitMeasurementScale = $scope.form.selectUnitMeasurementScale;

            selectUnitMeasurementScale.$setViewValue(selectedUnitMeasurementScale);
            selectUnitMeasurementScale.$commitViewValue();
            selectUnitMeasurementScale.$render();

        };

        var changeUnitMeasurementScale = debounce(500, function () {

            var unitMeasurement = $scope.unitMeasurementView.selected;
            var country = $scope.countryView.selected;
            var numericalScaleType = $scope.numericalScaleTypeView.selected;
            var unitMeasurementScale = $scope.unitMeasurementScaleView.selected;

            if (unitMeasurement && country && numericalScaleType && unitMeasurementScale) {                

                sensorUnitMeasurementScaleService.setUnitMeasurementNumericalScaleTypeCountry(
                    $scope.sensorUnitMeasurementScale.sensorUnitMeasurementScaleId,
                    $scope.sensorUnitMeasurementScale.sensorDatasheetId,
                    $scope.sensorUnitMeasurementScale.sensorTypeId,
                    unitMeasurementScale.unitMeasurementId,
                    unitMeasurementScale.unitMeasurementTypeId,
                    unitMeasurementScale.numericalScalePrefixId,
                    unitMeasurementScale.numericalScaleTypeId,
                    country.countryId
                );
            }

        });

        var clearOnSetUnitMeasurementNumericalScaleTypeCountryCompleted = null;
        //var clearOnSetValueCompleted = null;

        $scope.$on('$destroy', function () {
            clearOnSetUnitMeasurementNumericalScaleTypeCountryCompleted();
            //clearOnSetValueCompleted();
        });

        var onSetUnitMeasurementNumericalScaleTypeCountryCompleted = function (event, data) {
            finalizeSelectedWatches();
            applySelectsWithContext();
            initializeSelectedWatches();
            $scope.$apply();
            toaster.pop('success', 'Sucesso', 'UnitMeasurementNumericalScaleTypeCountry alterado');
        };

        //$scope.changeValue = function (position, value) {
        //    if (!initialized || value === undefined) return;
        //    var valueConverted = unitMeasurementConverter.convertToCelsius($scope.sensor.unitMeasurementId, value);
        //    sensorUnitMeasurementScaleService.setValue($scope.sensor.sensorUnitMeasurementScale.id, valueConverted, position);
        //};

        //$scope.$watch('sensor.sensorUnitMeasurementScale.maxConverted', function (newValue, oldValue) {
        //    $scope.maxView = $scope.sensor.sensorUnitMeasurementScale.maxConverted;
        //});

        //$scope.$watch('sensor.sensorUnitMeasurementScale.minConverted', function (newValue, oldValue) {
        //    $scope.minView = $scope.sensor.sensorUnitMeasurementScale.minConverted
        //});

        //var onSetValueCompleted = function (event, data) {
        //    if (data.position === 'Max') {
        //        toaster.pop('success', 'Sucesso', 'Limite alto do gráfico alterado');
        //    }
        //    else if (data.position === 'Min') {
        //        toaster.pop('success', 'Sucesso', 'Limite baixo do gráfico alterado');
        //    }
        //};

    }]);