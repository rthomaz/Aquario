﻿using RabbitMQ.Client;
using System.Threading.Tasks;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Producer.Interfaces;
using ART.Domotica.Constant;
using ART.Infra.CrossCutting.Utils;

namespace ART.Domotica.Producer.Services
{
    public class UnitOfMeasurementTypeProducer : ProducerBase, IUnitOfMeasurementTypeProducer
    {
        #region constructors

        public UnitOfMeasurementTypeProducer(IConnection connection) : base(connection)
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
                _model.BasicPublish("", UnitOfMeasurementTypeConstants.GetAllQueueName, null, payload);
            });            
        }

        #endregion

        #region private voids

        private void Initialize()
        {
            _model.QueueDeclare(
                  queue: UnitOfMeasurementTypeConstants.GetAllQueueName
                , durable: false
                , exclusive: false
                , autoDelete: true
                , arguments: null);            
        }

        #endregion
    }
}