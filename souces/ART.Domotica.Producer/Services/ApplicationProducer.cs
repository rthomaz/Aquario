﻿using System.Threading.Tasks;
using RabbitMQ.Client;
using ART.Infra.CrossCutting.MQ.Contract;
using ART.Infra.CrossCutting.MQ.Producer;
using ART.Domotica.Producer.Interfaces;
using ART.Domotica.Constant;
using ART.Infra.CrossCutting.Utils;
using ART.Domotica.Contract;
using ART.Infra.CrossCutting.MQ;
using RabbitMQ.Client.MessagePatterns;
using System;

namespace ART.Domotica.Producer.Services
{
    public class ApplicationProducer : ProducerBase, IApplicationProducer
    {
        #region Fields

        private readonly IMQSettings _mqSettings;

        #endregion Fields

        #region constructors

        public ApplicationProducer(IConnection connection, IMQSettings mqSettings) : base(connection)
        {
            _mqSettings = mqSettings;
            Initialize();
        }

        #endregion

        #region public voids

        public async Task<ApplicationGetRPCResponseContract> GetRPC(AuthenticatedMessageContract message)
        {
            return await Task.Run(() =>
            {
                var rpcClient = new SimpleRpcClient(_model, ApplicationConstants.GetRPCQueueName)
                {
                    TimeoutMilliseconds = _mqSettings.RpcClientTimeOutMilliSeconds
                };

                var body = SerializationHelpers.SerializeToJsonBufferAsync(message);

                rpcClient.TimedOut += (sender, e) =>
                {
                    throw new TimeoutException("Worker time out");
                };

                rpcClient.Disconnected += (sender, e) =>
                {
                    rpcClient.Close();
                    throw new Exception("Worker disconected");
                };

                var bufferResult = rpcClient.Call(body);

                rpcClient.Close();

                var result = SerializationHelpers.DeserializeJsonBufferToType<ApplicationGetRPCResponseContract>(bufferResult);

                return result;
            });
        }

        #endregion

        #region private voids

        private void Initialize()
        {

        }

        #endregion
    }
}