﻿namespace ART.Domotica.Contract
{
    using System;

    using ART.Domotica.Enums;

    public class DSFamilyTempSensorSetAlarmCelsiusRequestContract
    {
        #region Properties

        public decimal AlarmCelsius
        {
            get; set;
        }

        public Guid DSFamilyTempSensorId
        {
            get; set;
        }

        public SensorChartLimiterPositionEnum Position
        {
            get; set;
        }

        #endregion Properties
    }
}