﻿namespace ART.Corporativo.WebApi.Controllers
{
    using System.Collections.Generic;
    using System.Linq;
    using System.Security.Claims;
    using System.Web.Http;

    [Authorize]
    [RoutePrefix("api/minhaConta")]
    public class MinhaContaController : ApiController
    {
        #region Methods

        [Route("")]
        public IEnumerable<object> Get()
        {
            //string token = "";
            //Microsoft.Owin.Security.AuthenticationTicket ticket = Startup.OAuthBearerOptions.AccessTokenFormat.Unprotect(token);

            var identity = User.Identity as ClaimsIdentity;

            return identity.Claims.Select(c => new
            {
                Type = c.Type,
                Value = c.Value
            });
        }

        #endregion Methods
    }
}