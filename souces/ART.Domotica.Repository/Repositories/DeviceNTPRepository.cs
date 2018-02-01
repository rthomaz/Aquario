﻿namespace ART.Domotica.Repository.Repositories
{
    using System;
    using System.Threading.Tasks;
    using ART.Domotica.Enums;
    using ART.Domotica.Repository.Entities;
    using ART.Domotica.Repository.Interfaces;
    using ART.Infra.CrossCutting.Repository;

    public class DeviceNTPRepository : RepositoryBase<ARTDbContext, DeviceNTP, Guid>, IDeviceNTPRepository
    {
        #region Constructors

        public DeviceNTPRepository(ARTDbContext context)
            : base(context)
        {
        }

        #endregion Constructors

        public async Task<DeviceNTP> GetByKey(Guid deviceId, Guid deviceDatasheetId)
        {
            return await _context.DeviceNTP.FindAsync(deviceId, deviceDatasheetId);
        }
    }
}