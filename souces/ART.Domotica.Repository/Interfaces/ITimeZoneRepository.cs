﻿namespace ART.Domotica.Repository.Interfaces
{
    using System.Collections.Generic;
    using System.Threading.Tasks;

    using ART.Domotica.Repository.Entities;
    using ART.Infra.CrossCutting.Repository;

    public interface ITimeZoneRepository : IRepository<ARTDbContext, TimeZone, byte>
    {
        #region Methods

        Task<List<TimeZone>> GetAll();

        #endregion Methods
    }
}