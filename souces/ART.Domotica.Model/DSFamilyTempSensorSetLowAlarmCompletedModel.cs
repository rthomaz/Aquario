﻿namespace ART.Domotica.Model
{
    using System;

    public class DSFamilyTempSensorSetLowAlarmCompletedModel
    {
        #region Properties

        public Guid DSFamilyTempSensorId
        {
            get; set;
        }

        public decimal? LowAlarm
        {
            get; set;
        }

        #endregion Properties
    }
}