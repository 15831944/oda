using System;
using System.Text.Json.Serialization;

namespace RayonImport.Lib
{
    public class Folder
    {
        public Guid Id { get; set; }
        public DateTime? CreatedAt { get; set; }
        public DateTime? UpdatedAt { get; set; }
        public string Name { get; set; }

        public Guid OwnerId { get; set; }
        public virtual User Owner { get; set; }

        [JsonPropertyName("folderId")]
        public Guid? ParentFolderId { get; set; }
        public virtual Folder ParentFolder { get; set; }
    }
}