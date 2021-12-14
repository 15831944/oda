// <copyright file="DatabaseContext.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon
{
    using System;
    using Microsoft.EntityFrameworkCore;
    using Npgsql;
    using Rayon.Lib;
    using Rayon.Lib.Components;

    /// <summary>
    /// A database context using the Entity Framework.
    /// Slower than the raw database context but with more features and simpler to use.
    /// </summary>
    public partial class DatabaseContext : DbContext
    {
        private static readonly string DbConnectionString;

        static DatabaseContext()
        {
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileAccess>("file_access_enum");
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileOrigin>("file_origin_enum");
            NpgsqlConnection.GlobalTypeMapper.MapEnum<File.FileCategory>("file_category_enum");
            NpgsqlConnection.GlobalTypeMapper.MapEnum<Diff.EventTypeEnum>("diff_event_enum");
            var dbName = Environment.GetEnvironmentVariable("RAYON_DB_NAME");
            var dbHost = Environment.GetEnvironmentVariable("RAYON_DB_HOST");
            var dbPassword = Environment.GetEnvironmentVariable("RAYON_DB_PASSWORD");
            var dbUsername = Environment.GetEnvironmentVariable("RAYON_DB_USERNAME");
            var timeout = 300;
            var commandTimeout = 300;
            DbConnectionString = $"Host={dbHost};Database={dbName};Username={dbUsername};Password={dbPassword};Max Auto Prepare=0;No Reset On Close=true;Timeout={timeout};CommandTimeout={commandTimeout};";
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DatabaseContext"/> class.
        /// </summary>
        public DatabaseContext()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DatabaseContext"/> class.
        /// </summary>
        /// <param name="options"></param>
        public DatabaseContext(DbContextOptions<DatabaseContext> options)
            : base(options)
        {
        }

        public virtual DbSet<Commit> Commits { get; set; }

        public virtual DbSet<Component> Components { get; set; }

        public virtual DbSet<Diff> Diffs { get; set; }

        public virtual DbSet<Element> Elements { get; set; }

        public virtual DbSet<File> Files { get; set; }

        public virtual DbSet<Model> Models { get; set; }

        public virtual DbSet<User> Users { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            if (!optionsBuilder.IsConfigured)
            {
                optionsBuilder.UseNpgsql(DbConnectionString);
            }
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder
                .HasPostgresEnum<Diff.EventTypeEnum>()
                .HasPostgresEnum<File.FileAccess>()
                .HasPostgresEnum<File.FileOrigin>()
                .HasPostgresEnum<File.FileCategory>()
                .HasPostgresExtension("uuid-ossp")
                .HasAnnotation("Relational:Collation", "en_US.UTF-8");

            modelBuilder.Entity<Commit>(entity =>
            {
                entity.ToTable("commit");

                entity.Property(e => e.Id)
                    .ValueGeneratedNever()
                    .HasColumnName("id");

                entity.Property(e => e.AuthorId).HasColumnName("author_id");

                entity.Property(e => e.CreatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("created_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.Description)
                    .IsRequired()
                    .HasColumnName("description");

                entity.Property(e => e.LocalId).HasColumnName("local_id");

                entity.Property(e => e.ModelId).HasColumnName("model_id");

                entity.HasOne(d => d.Author)
                    .WithMany()
                    .HasForeignKey(d => d.AuthorId)
                    .HasConstraintName("FK_96ca08cdea1175b926864e67c3c");

                entity.HasOne(d => d.Model)
                    .WithMany(p => p.Commits)
                    .HasForeignKey(d => d.ModelId)
                    .HasConstraintName("FK_c623f964732fd057f936a7a541e");
            });

            modelBuilder.Entity<Component>(entity =>
            {
                entity.ToTable("component");

                entity.HasKey(d => new { d.Handle, d.ModelId, d.ComponentType })
                      .HasName("PK_65aff955441b46efe5dc54cd1b9");

                entity.Ignore(e => e.Value);

                entity.Property(e => e.Handle)
                    .IsRequired()
                    .HasColumnType("character varying")
                    .HasColumnName("handle");

                entity.Property(e => e.ModelId).HasColumnName("model_id");

                entity.Property(e => e.ComponentType)
                    .HasColumnName("component_type");

                entity.Property(e => e.UpdatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("updated_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.ValueAsString)
                    .HasColumnType("jsonb")
                    .HasColumnName("value");

                entity.HasOne(d => d.Element)
                    .WithMany(e => e.Components)
                    .HasForeignKey(d => new { d.Handle, d.ModelId })
                    .HasConstraintName("FK_e91becca3c92fb58b96e5b3ef72");
            });

            modelBuilder.Entity<Diff>(entity =>
            {
                entity.ToTable("diff");

                entity.Ignore(e => e.Prev);

                entity.Ignore(e => e.Current);

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasDefaultValueSql("uuid_generate_v4()");

                entity.Property(e => e.Handle)
                    .IsRequired()
                    .HasColumnType("character varying")
                    .HasColumnName("handle");

                entity.Property(e => e.CommitId).HasColumnName("commit_id");

                entity.Property(e => e.Event)
                    .HasColumnName("event");

                entity.Property(e => e.CurrentAsString)
                    .HasColumnType("jsonb")
                    .HasColumnName("current");

                entity.Property(e => e.PrevAsString)
                    .HasColumnType("jsonb")
                    .HasColumnName("prev");

                entity.HasOne(d => d.Commit)
                    .WithMany(p => p.Diffs)
                    .HasForeignKey(d => d.CommitId)
                    .HasConstraintName("FK_1bc0e37ef1a31b563f155b1e761");
            });

            modelBuilder.Entity<Element>(entity =>
            {
                entity.HasKey(e => new { e.Handle, e.ModelId })
                    .HasName("PK_1746a7a31726a31f3a1ace9edf6");

                entity.ToTable("element");

                entity.Property(e => e.Handle)
                    .HasMaxLength(46)
                    .HasColumnName("handle");

                entity.Property(e => e.ModelId).HasColumnName("model_id");

                entity.Property(e => e.CreatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("created_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.CreatedById).HasColumnName("created_by_id");

                entity.HasOne(d => d.CreatedBy)
                    .WithMany()
                    .HasForeignKey(d => d.CreatedById)
                    .HasConstraintName("FK_0f054b75ee9f879dafdedea8bed");

                entity.HasOne(d => d.Model)
                    .WithMany(p => p.Elements)
                    .HasForeignKey(d => d.ModelId)
                    .HasConstraintName("FK_d180841f19b7c464d9620b27f31");
            });

            modelBuilder.Entity<File>(entity =>
            {
                entity.ToTable("file");

                entity.Property(e => e.Id)
                    .ValueGeneratedNever()
                    .HasColumnName("id");

                entity.Property(e => e.CreatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("created_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasColumnName("name");

                entity.Property(e => e.Size)
                    .IsRequired()
                    .HasColumnName("size");

                entity.Property(e => e.Hidden)
                    .IsRequired()
                    .HasColumnName("hidden");

                entity.Property(e => e.Meta)
                    .HasColumnName("meta");

                entity.Property(e => e.Origin)
                    .IsRequired()
                    .HasColumnName("origin");

                entity.Property(e => e.Category)
                    .IsRequired()
                    .HasColumnName("category");

                entity.Property(e => e.Access)
                    .IsRequired()
                    .HasColumnName("access");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasColumnName("name");

                entity.Property(e => e.Directory)
                    .IsRequired()
                    .HasColumnName("directory");

                entity.Property(e => e.FolderId).HasColumnName("folder_id");

                entity.Property(e => e.OwnerId).HasColumnName("owner_id");

                entity.Property(e => e.UpdatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("updated_at")
                    .HasDefaultValueSql("now()");

                entity.HasOne(d => d.Owner)
                    .WithMany()
                    .HasForeignKey(d => d.OwnerId)
                    .HasConstraintName("FK_cccc7b3ca9b0c44aaa4ac0cacfb");
            });

            modelBuilder.Entity<Model>(entity =>
            {
                entity.ToTable("model");

                entity.HasIndex(e => e.OwnerId, "IDX_d03bf69f14040ae79f9109fc14");

                entity.Ignore(e => e.Components);

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasDefaultValueSql("uuid_generate_v4()");

                entity.Property(e => e.CreatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("created_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasColumnName("name");

                entity.Property(e => e.OwnerId).HasColumnName("owner_id");

                entity.Property(e => e.UpdatedAt)
                    .HasColumnType("timestamp with time zone")
                    .HasColumnName("updated_at")
                    .HasDefaultValueSql("now()");

                entity.Property(e => e.FolderId).HasColumnName("folder_id");
            });

            modelBuilder.Entity<User>(entity =>
            {
                entity.ToTable("user");

                entity.Property(e => e.Id)
                    .ValueGeneratedNever()
                    .HasColumnName("id");

                entity.Property(e => e.Email)
                    .IsRequired()
                    .HasColumnName("email");

                entity.Property(e => e.Username)
                    .IsRequired()
                    .HasColumnName("username");
            });

            this.OnModelCreatingPartial(modelBuilder);
        }

        partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
    }
}
