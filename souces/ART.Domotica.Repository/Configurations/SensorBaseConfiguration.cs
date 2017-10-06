﻿namespace ART.Domotica.Repository.Configurations
{
    using System.Data.Entity.ModelConfiguration;

    using ART.Domotica.Repository.Entities;

    public class SensorBaseConfiguration : EntityTypeConfiguration<SensorBase>
    {
        #region Constructors

        public SensorBaseConfiguration()
        {
            ToTable("SensorBase");

            //Primary Keys
            HasKey(x => x.Id);

            //Id
            Property(x => x.Id)
                .HasColumnOrder(0)
                .IsRequired();
        }

        #endregion Constructors
    }
}