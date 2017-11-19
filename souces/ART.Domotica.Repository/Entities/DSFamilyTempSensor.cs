﻿namespace ART.Domotica.Repository.Entities
{
    public class DSFamilyTempSensor : SensorBase
    {
        #region Properties

        public string DeviceAddress
        {
            get; set;
        }

        public DSFamilyTempSensorResolution DSFamilyTempSensorResolution
        {
            get; set;
        }

        public byte DSFamilyTempSensorResolutionId
        {
            get; set;
        }

        public string Family
        {
            get; set;
        }

        public TempSensorAlarm HighAlarm
        {
            get; set;
        }

        public TempSensorAlarm LowAlarm
        {
            get; set;
        }

        public TemperatureScale TemperatureScale
        {
            get; set;
        }

        public byte TemperatureScaleId
        {
            get; set;
        }

        public TempSensorRange TempSensorRange
        {
            get; set;
        }

        public byte TempSensorRangeId
        {
            get; set;
        }

        public decimal HighChartLimiterCelsius
        {
            get; set;
        }

        public decimal LowChartLimiterCelsius
        {
            get; set;
        }

        #endregion Properties
    }
}