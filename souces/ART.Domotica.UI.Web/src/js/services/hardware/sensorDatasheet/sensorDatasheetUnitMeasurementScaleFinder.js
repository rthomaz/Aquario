﻿'use strict';
app.factory('sensorDatasheetUnitMeasurementScaleFinder', ['$rootScope', 'sensorDatasheetContext', 'unitMeasurementFinder',
    function ($rootScope, sensorDatasheetContext, unitMeasurementFinder) {

        var context = sensorDatasheetContext;

        var serviceFactory = {};

        var getByKey = function (sensorDatasheetId, sensorTypeId, unitMeasurementId, unitMeasurementTypeId, numericalScalePrefixId, numericalScaleTypeId) {
            for (var i = 0; i < context.sensorDatasheetUnitMeasurementScale.length; i++) {
                var item = context.sensorDatasheetUnitMeasurementScale[i];
                if (item.sensorDatasheetId === sensorDatasheetId && item.sensorTypeId === sensorTypeId && item.unitMeasurementId === unitMeasurementId && item.unitMeasurementTypeId === unitMeasurementTypeId && item.numericalScalePrefixId === numericalScalePrefixId && item.numericalScaleTypeId === numericalScaleTypeId) {
                    return item;
                }
            }
        };

        var getBySensorDatasheetKey = function (sensorDatasheetId, sensorTypeId) {
            var result = [];
            for (var i = 0; i < context.sensorDatasheetUnitMeasurementScale.length; i++) {
                var sensorDatasheetUnitMeasurementScale = context.sensorDatasheetUnitMeasurementScale[i];
                if (sensorDatasheetUnitMeasurementScale.sensorDatasheetId === sensorDatasheetId && sensorDatasheetUnitMeasurementScale.sensorTypeId === sensorTypeId) {
                    result.push(sensorDatasheetUnitMeasurementScale);
                }
            }
            return result;
        };

        var getByUnitMeasurementScaleKey = function (unitMeasurementId, unitMeasurementTypeId, numericalScalePrefixId, numericalScaleTypeId) {
            var result = [];
            for (var i = 0; i < context.sensorDatasheetUnitMeasurementScale.length; i++) {
                var sensorDatasheetUnitMeasurementScale = context.sensorDatasheetUnitMeasurementScale[i];
                if (sensorDatasheetUnitMeasurementScale.unitMeasurementId === unitMeasurementId && sensorDatasheetUnitMeasurementScale.unitMeasurementTypeId === unitMeasurementTypeId && sensorDatasheetUnitMeasurementScale.numericalScalePrefixId === numericalScalePrefixId && sensorDatasheetUnitMeasurementScale.numericalScaleTypeId === numericalScaleTypeId) {
                    result.push(sensorDatasheetUnitMeasurementScale);
                }
            }
            return result;
        };

        var getUnitMeasurementsBySensorDatasheetKey = function (sensorDatasheetId, sensorTypeId) {
            var result = [];
            for (var i = 0; i < context.sensorDatasheetUnitMeasurementScale.length; i++) {
                var sensorDatasheetUnitMeasurementScale = context.sensorDatasheetUnitMeasurementScale[i];
                if (sensorDatasheetUnitMeasurementScale.sensorDatasheetId === sensorDatasheetId && sensorDatasheetUnitMeasurementScale.sensorTypeId === sensorTypeId) {
                    var unitMeasurement = unitMeasurementFinder.getByKey(sensorDatasheetUnitMeasurementScale.unitMeasurementId, sensorDatasheetUnitMeasurementScale.unitMeasurementTypeId);
                    for (var j = 0; j < result.length; j++) {
                        if (result[j] === unitMeasurement) break;
                    }
                    result.push(unitMeasurement);
                }
            }
            return result;
        }

        // *** Public Methods ***

        serviceFactory.getByKey = getByKey;
        serviceFactory.getBySensorDatasheetKey = getBySensorDatasheetKey;
        serviceFactory.getByUnitMeasurementScaleKey = getByUnitMeasurementScaleKey;
        serviceFactory.getUnitMeasurementsBySensorDatasheetKey = getUnitMeasurementsBySensorDatasheetKey;

        return serviceFactory;

    }]);