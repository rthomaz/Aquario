﻿namespace ART.Domotica.Repository
{
    using ART.Domotica.Repository.Interfaces;
    using ART.Domotica.Repository.Repositories;

    using Autofac;

    public class RepositoryModule : Module
    {
        #region Methods

        protected override void Load(ContainerBuilder builder)
        {
            builder.RegisterType<ApplicationRepository>().As<IApplicationRepository>();
            builder.RegisterType<ApplicationUserRepository>().As<IApplicationUserRepository>();
            builder.RegisterType<TemperatureScaleRepository>().As<ITemperatureScaleRepository>();
            builder.RegisterType<DSFamilyTempSensorRepository>().As<IDSFamilyTempSensorRepository>();
            builder.RegisterType<DSFamilyTempSensorResolutionRepository>().As<IDSFamilyTempSensorResolutionRepository>();
            builder.RegisterType<ThermometerDeviceRepository>().As<IThermometerDeviceRepository>();
        }

        #endregion Methods
    }
}