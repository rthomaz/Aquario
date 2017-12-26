﻿namespace ART.Domotica.Repository.Repositories
{
    using System;
    using System.Threading.Tasks;
    using ART.Domotica.Enums;
    using ART.Domotica.Repository.Entities;
    using ART.Domotica.Repository.Interfaces;
    using ART.Infra.CrossCutting.Repository;

    public class DeviceSensorsRepository : RepositoryBase<ARTDbContext, DeviceSensors>, IDeviceSensorsRepository
    {
        #region Constructors

        public DeviceSensorsRepository(ARTDbContext context)
            : base(context)
        {
        }

        #endregion Constructors

        public async Task<DeviceSensors> GetByKey(Guid deviceId, DeviceDatasheetEnum deviceDatasheetId)
        {
            return await _context.DeviceSensors.FindAsync(deviceId, deviceDatasheetId);
        }
    }
}