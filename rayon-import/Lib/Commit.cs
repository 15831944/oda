using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib
{
    public class Commit
    {
        public Commit()
        {
            this.Diffs = new HashSet<Diff>();
        }

        public Commit(string description, User author, Model model)
        {
            this.Diffs = new HashSet<Diff>();
            this.Description = description;
            this.Author = author;
            this.AuthorId = author.Id;
            this.Model = model;
            this.ModelId = model.Id;
            this.Id = Guid.NewGuid();
        }

        public Guid Id { get; set; }
        public int LocalId { get; set; }
        public string Description { get; set; }
        public DateTime? CreatedAt { get; set; }
        public Guid ModelId { get; set; }
        public Guid AuthorId { get; set; }

        public virtual User Author { get; set; }
        public virtual Model Model { get; set; }
        public virtual ICollection<Diff> Diffs { get; set; }
    }
}
