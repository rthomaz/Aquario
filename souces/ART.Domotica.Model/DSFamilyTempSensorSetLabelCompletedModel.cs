﻿namespace ART.Domotica.Model
{
    using System;

    public class DSFamilyTempSensorSetLabelCompletedModel
    {
        #region Properties

        public Guid DeviceId
        {
            get; set;
        }

        public Guid DSFamilyTempSensorId
        {
            get; set;
        }

        public string Label
        {
            get; set;
        }

        #endregion Properties
    }
}