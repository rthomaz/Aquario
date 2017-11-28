﻿namespace ART.Domotica.Repository.Entities
{
    using System.Collections.Generic;

    using ART.Domotica.Enums;

    public abstract class SensorBase : HardwareBase
    {
        #region Properties

        public ICollection<SensorsInDevice> SensorsInDevice
        {
            get; set;
        }

        public ICollection<SensorTrigger> SensorTriggers
        {
            get; set;
        }

        public UnitOfMeasurement UnitOfMeasurement
        {
            get; set;
        }

        public UnitOfMeasurementEnum UnitOfMeasurementId
        {
            get; set;
        }

        public UnitOfMeasurementTypeEnum UnitOfMeasurementTypeId
        {
            get; protected set;
        }

        #endregion Properties
    }
}