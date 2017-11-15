﻿using System.Web.Http;
using System.Threading.Tasks;
using ART.Domotica.Contract;
using ART.Infra.CrossCutting.MQ.WebApi;
using ART.Domotica.Producer.Interfaces;

namespace ART.Domotica.WebApi.Controllers
{
    [Authorize]
    [RoutePrefix("api/dsFamilyTempSensor")]    
    public class DSFamilyTempSensorController : AuthenticatedMQApiControllerBase
    {
        #region private readonly fields

        protected readonly IDSFamilyTempSensorProducer _dsFamilyTempSensorProducer;

        #endregion

        #region constructors

        public DSFamilyTempSensorController(IDSFamilyTempSensorProducer dsFamilyTempSensorProducer) 
        {
            _dsFamilyTempSensorProducer = dsFamilyTempSensorProducer;
        }

        #endregion

        #region public voids        
        
        /// <summary>
        /// Retornar uma lista de Resoluções
        /// </summary>        
        /// <remarks>
        /// Retornar uma lista de Resoluções
        /// </remarks>
        /// <response code="200">OK</response>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("getAllResolutions")]
        [HttpPost]
        public async Task<IHttpActionResult> GetAllResolutions()
        {
            await _dsFamilyTempSensorProducer.GetAllResolutions(CreateMessage());
            return Ok();
        }

        /// <summary>
        /// Altera a resolução de um sensor
        /// </summary>
        /// <remarks>
        /// Altera a resolução de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setResolution")]
        [HttpPost]
        public async Task<IHttpActionResult> SetResolution(DSFamilyTempSensorSetResolutionRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetResolution(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Altera a escala de um sensor
        /// </summary>
        /// <remarks>
        /// Altera a escala de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setScale")]
        [HttpPost]
        public async Task<IHttpActionResult> SetScale(DSFamilyTempSensorSetScaleRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetScale(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Liga o alarme de um sensor
        /// </summary>
        /// <remarks>
        /// Liga o alarme de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setAlarmOn")]
        [HttpPost]
        public async Task<IHttpActionResult> SetAlarmOn(DSFamilyTempSensorSetAlarmOnRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetAlarmOn(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Desliga o alarme de um sensor
        /// </summary>
        /// <remarks>
        /// Desliga o alarme de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setAlarmOff")]
        [HttpPost]
        public async Task<IHttpActionResult> SetAlarmOff(DSFamilyTempSensorSetAlarmOffRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetAlarmOff(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Altera o alarme alto de um sensor
        /// </summary>
        /// <remarks>
        /// Altera o alarme alto de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setHighAlarm")]
        [HttpPost]
        public async Task<IHttpActionResult> SetHighAlarm(DSFamilyTempSensorSetHighAlarmRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetHighAlarm(CreateMessage(contract));
            return Ok();
        }

        /// <summary>
        /// Altera o alarme baixo de um sensor
        /// </summary>
        /// <remarks>
        /// Altera o alarme baixo de um sensor
        /// </remarks>
        /// <param name="contract">contrato do request</param>
        /// <response code="400">Bad Request</response>
        /// <response code="403">Forbidden</response>
        /// <response code="500">Internal Server Error</response>
        [Route("setLowAlarm")]
        [HttpPost]
        public async Task<IHttpActionResult> SetLowAlarm(DSFamilyTempSensorSetLowAlarmRequestContract contract)
        {
            await _dsFamilyTempSensorProducer.SetLowAlarm(CreateMessage(contract));
            return Ok();
        }

        #endregion
    }
}