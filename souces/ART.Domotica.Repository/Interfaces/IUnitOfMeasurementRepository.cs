﻿namespace ART.Domotica.Repository.Interfaces
{
    using System.Collections.Generic;
    using System.Threading.Tasks;

    using ART.Domotica.Repository.Entities;
    using ART.Infra.CrossCutting.Repository;

    public interface IUnitOfMeasurementRepository : IRepository<ARTDbContext, UnitOfMeasurement, byte>
    {
        #region Methods

        Task<List<UnitOfMeasurement>> GetAll();

        #endregion Methods
    }
}