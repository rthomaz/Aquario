﻿namespace ART.Domotica.Model
{
    using System;

    using ART.Domotica.Enums;

    public class SensorUnitMeasurementScaleSetValueModel
    {
        #region Properties

        public PositionEnum Position
        {
            get; set;
        }

        public SensorDatasheetEnum SensorDatasheetId
        {
            get; set;
        }

        public SensorTypeEnum SensorTypeId
        {
            get; set;
        }

        public Guid SensorUnitMeasurementScaleId
        {
            get; set;
        }

        public decimal Value
        {
            get; set;
        }

        #endregion Properties
    }
}