﻿namespace ART.Domotica.Worker.AutoMapper
{
    using ART.Domotica.Contract;
    using ART.Domotica.IoTContract;
    using ART.Domotica.Model;
    using ART.Domotica.Repository.Entities;

    using global::AutoMapper;

    public class DSFamilyTempSensorProfile : Profile
    {
        #region Constructors

        public DSFamilyTempSensorProfile()
        {
            CreateMap<TempSensorAlarm, TempSensorAlarmResponseIoTContract>();

            CreateMap<DSFamilyTempSensor, DSFamilyTempSensorGetAllByDeviceInApplicationIdResponseIoTContract>()
                .ForMember(vm => vm.DeviceAddress, m => m.ResolveUsing(src => {
                    var split = src.DeviceAddress.Split(':');
                    var result = new short[8];
                    for (int i = 0; i < 8; i++)
                    {
                        result[i] = short.Parse(split[i]);
                    }
                    return result;
                }))
                .ForMember(vm => vm.ResolutionBits, m => m.MapFrom(x => x.DSFamilyTempSensorResolution.Bits))
                .ForMember(vm => vm.DSFamilyTempSensorId, m => m.MapFrom(x => x.Id));

            CreateMap<TempSensorAlarmPositionContract, TempSensorAlarmPositionIoTContract>();
            CreateMap<DSFamilyTempSensorSetScaleRequestContract, DSFamilyTempSensorSetScaleRequestIoTContract>();
            CreateMap<DSFamilyTempSensorSetResolutionRequestContract, DSFamilyTempSensorSetResolutionRequestIoTContract>();
            CreateMap<DSFamilyTempSensorSetAlarmOnRequestContract, DSFamilyTempSensorSetAlarmOnRequestIoTContract>();
            CreateMap<DSFamilyTempSensorSetAlarmValueRequestContract, DSFamilyTempSensorSetAlarmValueRequestIoTContract>();
            CreateMap<DSFamilyTempSensorSetAlarmBuzzerOnRequestContract, DSFamilyTempSensorSetAlarmBuzzerOnRequestIoTContract>();
            
            CreateMap<TempSensorAlarmPositionContract, TempSensorAlarmPositionModel>();
            CreateMap<DSFamilyTempSensorSetAlarmOnRequestContract, DSFamilyTempSensorSetAlarmOnCompletedModel>();
            CreateMap<DSFamilyTempSensorSetAlarmValueRequestContract, DSFamilyTempSensorSetAlarmValueCompletedModel>();
            CreateMap<DSFamilyTempSensorSetAlarmBuzzerOnRequestContract, DSFamilyTempSensorSetAlarmBuzzerOnCompletedModel>();

            CreateMap<DSFamilyTempSensor, DSFamilyTempSensorSetResolutionCompletedModel>()
                .ForMember(vm => vm.DSFamilyTempSensorId, m => m.MapFrom(x => x.Id))
                .ForMember(vm => vm.DSFamilyTempSensorResolutionId, m => m.MapFrom(x => x.DSFamilyTempSensorResolutionId));

            CreateMap<DSFamilyTempSensor, DSFamilyTempSensorSetScaleCompletedModel>()
                .ForMember(vm => vm.DSFamilyTempSensorId, m => m.MapFrom(x => x.Id))
                .ForMember(vm => vm.TemperatureScaleId, m => m.MapFrom(x => x.TemperatureScaleId));

            CreateMap<TempSensorAlarm, TempSensorAlarmGetDetailModel>();

            CreateMap<SensorsInDevice, DSFamilyTempSensorGetDetailModel>()
                .ForMember(vm => vm.DSFamilyTempSensorId, m => m.MapFrom(x => x.SensorBaseId))
                .ForMember(vm => vm.DSFamilyTempSensorResolutionId, m => m.MapFrom(x => ((DSFamilyTempSensor)x.SensorBase).DSFamilyTempSensorResolutionId))
                .ForMember(vm => vm.HighAlarm, m => m.MapFrom(x => ((DSFamilyTempSensor)x.SensorBase).HighAlarm))
                .ForMember(vm => vm.LowAlarm, m => m.MapFrom(x => ((DSFamilyTempSensor)x.SensorBase).LowAlarm))
                .ForMember(vm => vm.TemperatureScaleId, m => m.MapFrom(x => ((DSFamilyTempSensor)x.SensorBase).TemperatureScaleId));

            CreateMap<DSFamilyTempSensorResolution, DSFamilyTempSensorResolutionDetailModel>();
        }

        #endregion Constructors
    }
}