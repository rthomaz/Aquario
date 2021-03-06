﻿namespace ART.Domotica.IoTContract
{
    using System;

    using ART.Domotica.Enums;

    public class DeviceSerialSetPinRequestIoTContract
    {
        #region Properties

        public Guid DeviceSerialId
        {
            get; set;
        }

        public CommunicationDirection Direction
        {
            get; set;
        }

        public short Value
        {
            get; set;
        }

        #endregion Properties
    }
}