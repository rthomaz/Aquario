namespace ART.Domotica.Repository.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class InitialCreate11 : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.DeviceSerial", "PinRX", c => c.Short());
            AlterColumn("dbo.DeviceSerial", "PinTX", c => c.Short());
        }
        
        public override void Down()
        {
            AlterColumn("dbo.DeviceSerial", "PinTX", c => c.Int());
            AlterColumn("dbo.DeviceSerial", "PinRX", c => c.Int());
        }
    }
}
