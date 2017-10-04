﻿using Microsoft.AspNet.Identity.EntityFramework;
using System;

namespace ART.Seguranca.DistributedServices.Entities
{
    public class ApplicationUser : IdentityUser<Guid, ApplicationUserLogin, ApplicationUserRole, ApplicationUserClaim>
    {
    }
}