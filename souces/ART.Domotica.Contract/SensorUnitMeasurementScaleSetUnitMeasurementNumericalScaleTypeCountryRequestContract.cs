﻿namespace ART.Domotica.Contract
{
    using System;

    using ART.Domotica.Enums;
    using ART.Domotica.Enums.SI;

    public class SensorUnitMeasurementScaleSetUnitMeasurementNumericalScaleTypeCountryRequestContract
    {
        #region Properties

        public short CountryId
        {
            get; set;
        }

        public NumericalScalePrefixEnum NumericalScalePrefixId
        {
            get; set;
        }

        public NumericalScaleTypeEnum NumericalScaleTypeId
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

        public UnitMeasurementEnum UnitMeasurementId
        {
            get; set;
        }

        public UnitMeasurementTypeEnum UnitMeasurementTypeId
        {
            get; set;
        }

        #endregion Properties
    }
}