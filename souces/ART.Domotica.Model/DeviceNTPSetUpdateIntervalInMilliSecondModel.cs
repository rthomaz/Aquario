﻿namespace ART.Domotica.Model
{
    using System;

    using ART.Domotica.Enums;

    public class DeviceNTPSetUpdateIntervalInMilliSecondModel
    {
        #region Properties

        public DeviceDatasheetEnum DeviceDatasheetId
        {
            get; set;
        }

        public Guid DeviceNTPId
        {
            get; set;
        }

        public int UpdateIntervalInMilliSecond
        {
            get; set;
        }

        #endregion Properties
    }
}