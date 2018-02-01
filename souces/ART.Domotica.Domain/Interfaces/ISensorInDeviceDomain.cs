﻿namespace ART.Domotica.Domain.Interfaces
{
    using System;
    using System.Threading.Tasks;

    using ART.Domotica.Enums;
    using ART.Domotica.Repository.Entities;

    public interface ISensorInDeviceDomain
    {
        #region Methods

        Task<SensorInDevice> SetOrdination(Guid deviceSensorsId, Guid deviceDatasheetId, Guid sensorId, SensorDatasheetEnum sensorDatasheetId, SensorTypeEnum sensorTypeId, short ordination);

        #endregion Methods
    }
}