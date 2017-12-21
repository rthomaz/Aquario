﻿namespace ART.Domotica.Worker.AutoMapper
{
    using ART.Domotica.Model;
    using ART.Domotica.Repository.Entities;

    using global::AutoMapper;

    public class SensorDatasheetProfile : Profile
    {
        #region Constructors

        public SensorDatasheetProfile()
        {
            CreateMap<SensorDatasheet, SensorDatasheetGetModel>()
                .ForMember(vm => vm.SensorDatasheetId, m => m.MapFrom(x => x.Id))
                .ForMember(vm => vm.SensorTypeId, m => m.MapFrom(x => x.SensorTypeId))
                .ForMember(vm => vm.Name, m => m.MapFrom(x => x.Name));
        }

        #endregion Constructors
    }
}