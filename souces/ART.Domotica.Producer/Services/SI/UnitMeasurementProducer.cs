﻿using RabbitMQ.Client;
using System.Threading.Tasks;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Constant.SI;
using ART.Domotica.Producer.Interfaces.SI;
using ART.Infra.CrossCutting.MQ;

namespace ART.Domotica.Producer.Services.SI
{
    public class UnitMeasurementProducer : ProducerBase, IUnitMeasurementProducer
    {
        #region constructors

        public UnitMeasurementProducer(IConnection connection, IMQSettings mqSettings)
            : base(connection, mqSettings)
        {
            Initialize();
        }

        #endregion

        #region public voids

        public async Task GetAll(AuthenticatedMessageContract message)
        {
            await BasicPublish(UnitMeasurementConstants.GetAllQueueName, message);
        }

        #endregion

        #region private voids

        private void Initialize()
        {
            _model.QueueDeclare(
                  queue: UnitMeasurementConstants.GetAllQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: CreateBasicArguments());            
        }

        #endregion
    }
}