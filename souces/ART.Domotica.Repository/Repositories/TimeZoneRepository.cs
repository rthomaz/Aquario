﻿using ART.Domotica.Repository.Entities;
using ART.Domotica.Repository.Interfaces;
using ART.Infra.CrossCutting.Repository;
using System.Collections.Generic;
using System.Data.Entity;
using System.Threading.Tasks;

namespace ART.Domotica.Repository.Repositories
{
    public class TimeZoneRepository : RepositoryBase<ARTDbContext, TimeZone, byte>, ITimeZoneRepository
    {
        public TimeZoneRepository(ARTDbContext context) : base(context)
        {

        }

        public async Task<List<TimeZone>> GetAll()
        {
            return await _context.TimeZone                
                .ToListAsync();
        }
    }
}