// <copyright file="Commit.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    public class Commit
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Commit"/> class.
        /// </summary>
        public Commit()
        {
            this.Diffs = "[]";
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Commit"/> class.
        /// </summary>
        /// <param name="description"></param>
        /// <param name="author"></param>
        /// <param name="model"></param>
        public Commit(string description, User author, Model model)
        {
            this.Description = description;
            this.Author = author;
            this.AuthorId = author.Id;
            this.Model = model;
            this.ModelId = model.Id;
            this.Diffs = "[]";
        }

        public int? Id { get; set; }

        public int LocalId { get; set; }

        public string Description { get; set; }

        public DateTime? CreatedAt { get; set; }

        public Guid ModelId { get; set; }

        public Guid AuthorId { get; set; }

        public virtual User Author { get; set; }

        public virtual Model Model { get; set; }

        public virtual string Diffs { get; set; }
    }
}
