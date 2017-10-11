﻿namespace ART.Domotica.Worker.Consumers
{
    using ART.Domotica.Constant;
    using ART.Domotica.Contract;
    using ART.Domotica.Domain.Interfaces;
    using ART.Domotica.Worker.IConsumers;
    using ART.Infra.CrossCutting.MQ.Contract;
    using ART.Infra.CrossCutting.MQ.Worker;
    using ART.Infra.CrossCutting.Utils;
    using RabbitMQ.Client;
    using RabbitMQ.Client.Events;
    using System.Threading.Tasks;

    public class HardwaresInApplicationConsumer : ConsumerBase, IHardwaresInApplicationConsumer
    {
        #region Fields

        private readonly EventingBasicConsumer _getListConsumer;        
        private readonly EventingBasicConsumer _searchPinConsumer;
        private readonly EventingBasicConsumer _insertHardwareConsumer;
        private readonly EventingBasicConsumer _deleteHardwareConsumer;

        private readonly IHardwaresInApplicationDomain _hardwaresInApplicationDomain;

        #endregion Fields

        #region Constructors

        public HardwaresInApplicationConsumer(IConnection connection, IHardwaresInApplicationDomain hardwaresInApplicationDomain)
            : base(connection)
        {
            _getListConsumer = new EventingBasicConsumer(_model);
            _searchPinConsumer = new EventingBasicConsumer(_model);
            _insertHardwareConsumer = new EventingBasicConsumer(_model);
            _deleteHardwareConsumer = new EventingBasicConsumer(_model);

            _hardwaresInApplicationDomain = hardwaresInApplicationDomain;

            Initialize();
        }

        #endregion Constructors

        #region Methods

        private void Initialize()
        {
            _model.QueueDeclare(
                 queue: HardwaresInApplicationConstants.GetListQueueName
               , durable: false
               , exclusive: false
               , autoDelete: true
               , arguments: null);

            _model.QueueDeclare(
                 queue: HardwaresInApplicationConstants.SearchPinQueueName
               , durable: false
               , exclusive: false
               , autoDelete: true
               , arguments: null);

            _model.QueueDeclare(
                 queue: HardwaresInApplicationConstants.InsertHardwareQueueName
               , durable: false
               , exclusive: false
               , autoDelete: true
               , arguments: null);

            _model.QueueDeclare(
                 queue: HardwaresInApplicationConstants.DeleteHardwareQueueName
               , durable: false
               , exclusive: false
               , autoDelete: true
               , arguments: null);

            _getListConsumer.Received += GetListReceived;
            _searchPinConsumer.Received += SearchPinReceived;
            _insertHardwareConsumer.Received += InsertHardwareReceived;
            _deleteHardwareConsumer.Received += DeleteHardwareReceived;

            _model.BasicConsume(HardwaresInApplicationConstants.GetListQueueName, false, _getListConsumer);
            _model.BasicConsume(HardwaresInApplicationConstants.SearchPinQueueName, false, _searchPinConsumer);
            _model.BasicConsume(HardwaresInApplicationConstants.InsertHardwareQueueName, false, _insertHardwareConsumer);
            _model.BasicConsume(HardwaresInApplicationConstants.DeleteHardwareQueueName, false, _deleteHardwareConsumer);
        }

        #endregion Methods

        #region private voids

        public void GetListReceived(object sender, BasicDeliverEventArgs e)
        {
            Task.WaitAll(GetListReceivedAsync(sender, e));
        }
        public async Task GetListReceivedAsync(object sender, BasicDeliverEventArgs e)
        {
            _model.BasicAck(e.DeliveryTag, false);
            var message = SerializationHelpers.DeserializeJsonBufferToType<AuthenticatedMessageContract>(e.Body);
            var data = await _hardwaresInApplicationDomain.GetList(message);
            var buffer = SerializationHelpers.SerializeToJsonBufferAsync(data);
            var exchange = "amq.topic";
            var rountingKey = string.Format("{0}-{1}", message.SouceMQSession, HardwaresInApplicationConstants.GetListCompletedQueueName);
            _model.BasicPublish(exchange, rountingKey, null, buffer);
        }

        public void SearchPinReceived(object sender, BasicDeliverEventArgs e)
        {
            Task.WaitAll(SearchPinReceivedAsync(sender, e));
        }

        public async Task SearchPinReceivedAsync(object sender, BasicDeliverEventArgs e)
        {
            _model.BasicAck(e.DeliveryTag, false);
            var message = SerializationHelpers.DeserializeJsonBufferToType<AuthenticatedMessageContract<HardwaresInApplicationPinContract>>(e.Body);
            var data = await _hardwaresInApplicationDomain.SearchPin(message);
            var buffer = SerializationHelpers.SerializeToJsonBufferAsync(data);
            var exchange = "amq.topic";
            var rountingKey = string.Format("{0}-{1}", message.SouceMQSession, HardwaresInApplicationConstants.SearchPinCompletedQueueName);
            _model.BasicPublish(exchange, rountingKey, null, buffer);
        }

        public void InsertHardwareReceived(object sender, BasicDeliverEventArgs e)
        {
            Task.WaitAll(InsertHardwareReceivedAsync(sender, e));
        }

        public async Task InsertHardwareReceivedAsync(object sender, BasicDeliverEventArgs e)
        {
            _model.BasicAck(e.DeliveryTag, false);
            var message = SerializationHelpers.DeserializeJsonBufferToType<AuthenticatedMessageContract<HardwaresInApplicationPinContract>>(e.Body);
            await _hardwaresInApplicationDomain.InsertHardware(message);            
            var exchange = "amq.topic";
            var rountingKey = string.Format("{0}-{1}", message.SouceMQSession, HardwaresInApplicationConstants.InsertHardwareCompletedQueueName);
            _model.BasicPublish(exchange, rountingKey, null, null);
        }

        public void DeleteHardwareReceived(object sender, BasicDeliverEventArgs e)
        {
            Task.WaitAll(DeleteHardwareReceivedAsync(sender, e));
        }

        public async Task DeleteHardwareReceivedAsync(object sender, BasicDeliverEventArgs e)
        {
            _model.BasicAck(e.DeliveryTag, false);
            var message = SerializationHelpers.DeserializeJsonBufferToType<AuthenticatedMessageContract<HardwaresInApplicationDeleteHardwareContract>>(e.Body);
            await _hardwaresInApplicationDomain.DeleteHardware(message);
            var exchange = "amq.topic";
            var rountingKey = string.Format("{0}-{1}", message.SouceMQSession, HardwaresInApplicationConstants.DeleteHardwareCompletedQueueName);
            _model.BasicPublish(exchange, rountingKey, null, null);
        }

        #endregion Other
    }
}