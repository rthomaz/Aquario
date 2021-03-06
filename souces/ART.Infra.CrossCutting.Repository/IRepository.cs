﻿namespace ART.Infra.CrossCutting.Repository
{
    using System;
    using System.Collections.Generic;
    using System.Data.Entity;
    using System.Linq;
    using System.Linq.Expressions;
    using System.Threading.Tasks;

    public interface IRepository<TDbContext, TEntity>
        where TDbContext : DbContext
        where TEntity : IEntity
    {
        #region Methods

        Task Delete(TEntity entity);

        Task Delete(List<TEntity> entities);

        Task Insert(TEntity entity);

        Task Insert(List<TEntity> entities);

        IQueryable<TEntity> SearchFor(Expression<Func<TEntity, bool>> predicate);

        Task Update(TEntity entity);

        Task Update(List<TEntity> entities);

        #endregion Methods
    }

    public interface IRepository<TDbContext, TEntity, TKey> : IRepository<TDbContext, TEntity>
        where TDbContext : DbContext
        where TEntity : IEntity<TKey>
        where TKey : struct
    {
        #region Methods

        Task<TEntity> GetByKey(TKey key);

        #endregion Methods
    }
}