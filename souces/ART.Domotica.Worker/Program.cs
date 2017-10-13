﻿namespace ART.Domotica.Worker
{
    using System;
    using System.Configuration;

    using ART.Domotica.Domain;
    using ART.Domotica.Domain.AutoMapper;
    using ART.Domotica.Job;
    using ART.Domotica.Repository;
    using ART.Domotica.Worker.Modules;
    using ART.Infra.CrossCutting.Logging;
    using ART.Infra.CrossCutting.MQ;
    using ART.Infra.CrossCutting.Scheduler;
    using ART.Infra.CrossCutting.Setting;

    using Autofac;

    using global::AutoMapper;

    using Quartz;
    using Quartz.Spi;

    using Topshelf;
    using Topshelf.Autofac;
    using Topshelf.Quartz;

    class Program
    {
        #region Methods

        private static int GetChangePinIntervalInSeconds(IContainer container)
        {
            var settingManager = container.Resolve<ISettingManager>();

            var changePinIntervalInSecondsSettingsKey = "ChangePinIntervalInSeconds";

            var exists = settingManager.Exist(changePinIntervalInSecondsSettingsKey);

            int changePinIntervalInSeconds;

            if (exists)
            {
                changePinIntervalInSeconds = settingManager.GetValue<int>(changePinIntervalInSecondsSettingsKey);
            }
            else
            {
                var changePinIntervalInSecondsDefault = Convert.ToInt32(ConfigurationManager.AppSettings["ChangePinIntervalInSecondsDefault"]);
                settingManager.Insert(changePinIntervalInSecondsSettingsKey, changePinIntervalInSecondsDefault);
                changePinIntervalInSeconds = changePinIntervalInSecondsDefault;
            }

            return changePinIntervalInSeconds;
        }

        static void Main(string[] args)
        {
            log4net.Config.XmlConfigurator.Configure();

            var builder = new ContainerBuilder();

            builder.RegisterType<WorkerService>();

            builder.RegisterType<ARTDbContext>().InstancePerDependency();

            // CrossCutting Modules

            builder.RegisterModule<LoggingModule>();
            builder.RegisterModule<SettingModule>();

            builder.RegisterModule<RepositoryModule>();
            builder.RegisterModule<DomainModule>();
            builder.RegisterModule<MQModule>();
            builder.RegisterModule<ConsumerModule>();
            builder.RegisterModule<JobModule>();

            builder.RegisterModule<SchedulerModule>();

            Mapper.Initialize(x =>
            {
                x.AddProfile(new ApplicationProfile());
                x.AddProfile(new ApplicationUserProfile());
                x.AddProfile(new DSFamilyTempSensorProfile());
                x.AddProfile(new HardwaresInApplicationProfile());
                x.AddProfile(new TemperatureScaleProfile());
                x.AddProfile(new ThermometerDeviceProfile());
            });

            IContainer container = builder.Build();

            ScheduleJobServiceConfiguratorExtensions.SchedulerFactory = () => container.Resolve<IScheduler>();

            HostFactory.Run(x =>
            {
                x.UseAssemblyInfoForServiceInfo();

                x.UseAutofacContainer(container);

                x.Service<WorkerService>(s =>
                {
                    s.ConstructUsingAutofacContainer();
                    s.WhenStarted((service, control) => service.Start());
                    s.WhenStopped((service, control) => service.Stop());
                });

                x.SetStartTimeout(TimeSpan.FromSeconds(10));
                x.SetStopTimeout(TimeSpan.FromSeconds(10));

                x.EnableServiceRecovery(r =>
                {
                    r.RestartService(3);
                    //r.RunProgram(7, "ping google.com");
                    //r.RestartComputer(5, "message");
                    r.OnCrashOnly();
                    r.SetResetPeriod(2);
                });

                x.OnException((exception) =>
                {
                    Console.WriteLine("Exception thrown - " + exception.Message);
                });

                x.UsingQuartzJobFactory(() => container.Resolve<IJobFactory>());

                x.ScheduleQuartzJobAsService(q =>
                {
                    q.WithJob(() => JobBuilder.Create<UpdatePinJob>().Build());
                    q.AddTrigger(() => TriggerBuilder.Create().WithSimpleSchedule(b =>
                    {
                        var interval = GetChangePinIntervalInSeconds(container);
                        b.WithIntervalInSeconds(interval).RepeatForever();
                    }).Build());
                });

            });
        }

        #endregion Methods
    }
}