// <copyright file="RawDatabaseContext.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text.Json;
    using Npgsql;
    using NpgsqlTypes;
    using Rayon.Core;
    using Rayon.Core.Components;

    /// <summary>
    /// A Raw database context for fast operation in the db. Much faster than the EntityFramework database context.
    /// </summary>
    public class RawDatabaseContext
    {
        private static readonly string DbConnectionString;

        static RawDatabaseContext()
        {
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileAccess>("file_access_enum");
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileOrigin>("file_origin_enum");
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileCategory>("file_category_enum");
            var dbName = Environment.GetEnvironmentVariable("RAYON_DB_NAME");
            var dbHost = Environment.GetEnvironmentVariable("RAYON_DB_HOST");
            var dbPassword = Environment.GetEnvironmentVariable("RAYON_DB_PASSWORD");
            var dbUsername = Environment.GetEnvironmentVariable("RAYON_DB_USERNAME");
            var timeout = 300;
            var commandTimeout = 300;
            var keepalive = 300;
            DbConnectionString = $"Host={dbHost};Database={dbName};Username={dbUsername};Password={dbPassword};Timeout={timeout};CommandTimeout={commandTimeout};KeepAlive={keepalive};";
        }

        public NpgsqlConnection Connection()
        {
            return new NpgsqlConnection(RawDatabaseContext.DbConnectionString);
        }

        public void InsertComponents(ICollection<Component> components, NpgsqlConnection connection)
        {
            // To speed up the insertion we split the components in chuncks of 10 000 units.
            // This significantly improves the insertion time (by a factor of 6) but I have no idea why.
            // TODO : benchmark smaller or higher values to find optimal chunck size
            int n = 10000;
            int k = components.Count();
            int numBlocks = k / n;

            for (int i = 0; i <= numBlocks; i++)
            {
                // We use the COPY instruction for faster inserts
                using var writer = connection.BeginBinaryImport("COPY component (handle, model_id, component_type, value) FROM STDIN (FORMAT BINARY)");
                Console.WriteLine($"Saving component chunck {i} / {numBlocks}");

                foreach (Component component in components.Skip(i * n).Take(n))
                {
                    writer.StartRow();
                    writer.Write(component.Handle, NpgsqlDbType.Varchar);
                    writer.Write(component.ModelId, NpgsqlDbType.Uuid);
                    writer.Write((int)component.ComponentType, NpgsqlDbType.Integer);
                    writer.Write(JsonSerializer.Serialize(component.Value), NpgsqlDbType.Jsonb);
                }

                writer.Complete();
            }
        }

        public void InsertElements(ICollection<Element> elements, NpgsqlConnection connection)
        {
            // We use the COPY instruction for faster inserts
            using var writer = connection.BeginBinaryImport("COPY element (handle, model_id, created_by_id) FROM STDIN (FORMAT BINARY)");
            foreach (Element element in elements)
            {
                writer.StartRow();
                writer.Write(element.Handle, NpgsqlDbType.Varchar);
                writer.Write(element.ModelId, NpgsqlDbType.Uuid);
                writer.Write(element.CreatedById, NpgsqlDbType.Uuid);
            }

            writer.Complete();
        }

        public async void Analyze(NpgsqlConnection connection)
        {
            // It could be better to call analyze after bulk inserting
            // but this takes a lot of time.
            // see : https://www.2ndquadrant.com/en/blog/7-best-practice-tips-for-postgresql-bulk-data-loading/
            using var cmd = new NpgsqlCommand("ANALYZE", connection);
            await cmd.ExecuteNonQueryAsync();
        }

        /// <summary>
        /// TODO : implement this
        /// </summary>
        /// <param name="model"></param>
        public void InsertModel(Model model)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///  TODO : implement this
        /// </summary>
        /// <param name="modelId"></param>
        public void GetComponents(string modelId)
        {
            throw new NotImplementedException();
        }
    }
}
