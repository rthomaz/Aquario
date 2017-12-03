﻿using ART.Domotica.Contract;
using RabbitMQ.Client;
using System.Threading.Tasks;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Producer.Interfaces;
using ART.Domotica.Constant;
using ART.Infra.CrossCutting.Utils;

namespace ART.Domotica.Producer.Services
{
    public class SensorProducer : ProducerBase, ISensorProducer
    {
        #region constructors

        public SensorProducer(IConnection connection) : base(connection)
        {
            Initialize();
        }

        #endregion

        #region public voids

        public async Task GetAll(AuthenticatedMessageContract message)
        {
            await Task.Run(() =>
            {
                var payload = SerializationHelpers.SerializeToJsonBufferAsync(message);
                _model.BasicPublish("", SensorConstants.GetAllQueueName, null, payload);
            });            
        }

        public async Task SetUnitMeasurement(AuthenticatedMessageContract<SensorSetUnitMeasurementRequestContract> message)
        {
            await Task.Run(() =>
            {
                var payload = SerializationHelpers.SerializeToJsonBufferAsync(message);
                _model.BasicPublish("", SensorConstants.SetUnitMeasurementQueueName, null, payload);
            });
        }

        public async Task SetLabel(AuthenticatedMessageContract<SensorSetLabelRequestContract> message)
        {
            await Task.Run(() =>
            {
                var payload = SerializationHelpers.SerializeToJsonBufferAsync(message);
                _model.BasicPublish("", SensorConstants.SetLabelQueueName, null, payload);
            });
        }

        #endregion

        #region private voids

        private void Initialize()
        {
            _model.QueueDeclare(
                  queue: SensorConstants.GetAllQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: null);

            _model.QueueDeclare(
                 queue: SensorConstants.SetUnitMeasurementQueueName
               , durable: true
               , exclusive: false
               , autoDelete: false
               , arguments: null);

            _model.QueueDeclare(
                 queue: SensorConstants.SetLabelQueueName
               , durable: true
               , exclusive: false
               , autoDelete: false
               , arguments: null);
        }

        #endregion
    }
}