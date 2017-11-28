﻿namespace ART.Domotica.Repository.Entities
{
    using System.Collections.Generic;

    using ART.Infra.CrossCutting.Repository;

    public class SensorRange : IEntity<byte>
    {
        #region Properties

        public ICollection<SensorBase> Sensors
        {
            get; set;
        }

        public byte Id
        {
            get; set;
        }

        public short Max
        {
            get; set;
        }

        public short Min
        {
            get; set;
        }

        #endregion Properties
    }
}