﻿using ART.Domotica.Contract;
using RabbitMQ.Client;
using System.Threading.Tasks;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Producer.Interfaces;
using ART.Domotica.Constant;

namespace ART.Domotica.Producer.Services
{
    public class SensorTempDSFamilyProducer : ProducerBase, ISensorTempDSFamilyProducer
    {
        #region constructors

        public SensorTempDSFamilyProducer(IConnection connection) : base(connection)
        {
            Initialize();
        }

        #endregion

        #region public voids  

        public async Task GetAllResolutions(AuthenticatedMessageContract message)
        {
            await BasicPublish(SensorTempDSFamilyConstants.GetAllResolutionsQueueName, message);
        }

        public async Task SetResolution(AuthenticatedMessageContract<SensorTempDSFamilySetResolutionRequestContract> message)
        {
            await BasicPublish(SensorTempDSFamilyConstants.SetResolutionQueueName, message);
        }        

        #endregion

        #region private voids

        private void Initialize()
        {
            _model.QueueDeclare(
                  queue: SensorTempDSFamilyConstants.GetAllResolutionsQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: CreateBasicArguments());

            _model.QueueDeclare(
                  queue: SensorTempDSFamilyConstants.SetResolutionQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: CreateBasicArguments());
        }

        #endregion
    }
}