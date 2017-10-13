﻿namespace ART.Domotica.Domain.AutoMapper
{
    using ART.Domotica.Model;
    using ART.Domotica.Repository.Entities;

    using global::AutoMapper;

    public class HardwareProfile : Profile
    {
        #region Constructors

        public HardwareProfile()
        {
            CreateMap<HardwareBase, HardwareUpdatePinsModel>()
                .ForMember(vm => vm.HardwareId, m => m.MapFrom(x => x.Id));
        }

        #endregion Constructors
    }
}