﻿using ART.Data.Domain.Interfaces;
using ART.MQ.Common.Contracts;
using MassTransit;
using System.Threading.Tasks;

namespace ART.MQ.Worker.DSFamilyTempSensor
{
    public class DSFamilyTempSensorSetResolutionConsumer : IConsumer<DSFamilyTempSensorSetResolutionContract>
    {
        #region private fields

        private readonly IDSFamilyTempSensorDomain _dsFamilyTempSensorDomain;

        #endregion

        #region constructors

        public DSFamilyTempSensorSetResolutionConsumer(IDSFamilyTempSensorDomain dsFamilyTempSensorDomain)
        {
            _dsFamilyTempSensorDomain = dsFamilyTempSensorDomain;
        }

        #endregion

        #region public void

        public async Task Consume(ConsumeContext<DSFamilyTempSensorSetResolutionContract> context)
        {
            //await Console.Out.WriteLineAsync($"[DSFamilyTempSensor][SetResolution] deviceAddress:{context.Message.DeviceAddress} value: {context.Message.Value} ");            
            //await _dsFamilyTempSensorDomain.SetResolution(context.Message.DeviceAddress, context.Message.Value);
            //await context.Publish("");
        } 

        #endregion
    }
}
