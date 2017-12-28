﻿using RabbitMQ.Client;
using System.Threading.Tasks;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Producer.Interfaces;
using ART.Domotica.Constant;

namespace ART.Domotica.Producer.Services
{
    public class SensorDatasheetUnitMeasurementDefaultProducer : ProducerBase, ISensorDatasheetUnitMeasurementDefaultProducer
    {
        #region constructors

        public SensorDatasheetUnitMeasurementDefaultProducer(IConnection connection) : base(connection)
        {
            Initialize();
        }

        #endregion

        #region public voids

        public async Task GetAll(AuthenticatedMessageContract message)
        {
            await BasicPublish(SensorDatasheetUnitMeasurementDefaultConstants.GetAllQueueName, message);
        }

        #endregion

        #region private voids

        private void Initialize()
        {
            _model.QueueDeclare(
                  queue: SensorDatasheetUnitMeasurementDefaultConstants.GetAllQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: CreateBasicArguments());            
        }

        #endregion
    }
}