﻿namespace ART.Domotica.WebApi.Controllers
{
    using System.Web.Http;
    using ART.Infra.CrossCutting.MQ.WebApi;
    using System.Threading.Tasks;
    using ART.Domotica.Producer.Interfaces;
    using ART.Domotica.Contract;

    [Authorize]
    [RoutePrefix("api/hardwaresInApplication")]
    public class HardwaresInApplicationController : AuthenticatedMQApiControllerBase
    {
        #region Fields

        protected readonly IHardwaresInApplicationProducer _hardwaresInApplicationProducer;

        #endregion Fields

        #region Constructors

        //: base(connection)
        public HardwaresInApplicationController(IHardwaresInApplicationProducer hardwaresInApplicationProducer)
        {
            _hardwaresInApplicationProducer = hardwaresInApplicationProducer;
        }

        #endregion Constructors

        #region public voids

        /// <summary>
        /// Retornar um hardware pelo pin
        /// </summary>        
        /// <remarks>
        /// Retornar um hardware pelo pin
        /// </remarks>
        /// <response code="200">OK</response>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("searchPin")]
        [HttpPost]
        public async Task<IHttpActionResult> SearchPin(HardwaresInApplicationPinContract contract)
        {
            await _hardwaresInApplicationProducer.SearchPin(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Adiciona um hardware pelo pin
        /// </summary>        
        /// <remarks>
        /// Adiciona um hardware pelo pin
        /// </remarks>
        /// <response code="200">OK</response>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("insertHardware")]
        [HttpPost]
        public async Task<IHttpActionResult> InsertHardware(HardwaresInApplicationPinContract contract)
        {
            await _hardwaresInApplicationProducer.InsertHardware(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Remove um hardware pelo id
        /// </summary>        
        /// <remarks>
        /// Remove um hardware pelo id
        /// </remarks>
        /// <response code="200">OK</response>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("deleteHardware")]
        [HttpPost]
        public async Task<IHttpActionResult> deleteHardware(HardwaresInApplicationDeleteHardwareContract contract)
        {
            await _hardwaresInApplicationProducer.DeleteHardware(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Retornar uma lista de hardwares da aplicação
        /// </summary>        
        /// <remarks>
        /// Retornar uma lista de hardwares da aplicação
        /// </remarks>
        /// <response code="200">OK</response>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("getList")]
        [HttpPost]
        public async Task<IHttpActionResult> GetList()
        {
            await _hardwaresInApplicationProducer.GetList(CreateMessage());
            return Ok();
        }

        #endregion
    }
}