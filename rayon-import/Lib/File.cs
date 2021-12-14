using NpgsqlTypes;
using System;
using System.Text.Json.Serialization;

namespace RayonImport.Lib
{
    public class File
    {
        public static string PREFIX = "-RYPRFX---";

        public Guid Id { get; set; }
        public DateTime? CreatedAt { get; set; }
        public DateTime? UpdatedAt { get; set; }
        public Guid OwnerId { get; set; }
        public virtual User Owner { get; set; }

        public Guid? FolderId { get; set; }
        public virtual Folder Folder { get; set; }

        public string Name { get; set; }

        public int Size { get; set; }

        public bool Hidden { get; set; }

        public string Meta { get; set; }

        [JsonConverter(typeof(JsonStringEnumConverter))]
        public FileAccess Access { get; set; }

        [JsonConverter(typeof(JsonStringEnumConverter))]
        public FileOrigin Origin { get; set; }

        [JsonConverter(typeof(JsonStringEnumConverter))]
        public FileCategory Category { get; set; }

        public string Directory { get; set; }

        public enum FileAccess
        {
            [PgName("Public")]
            Public,

            [PgName("Protected")]
            Protected,

            [PgName("Private")]
            Private,
        }

        public enum FileOrigin
        {
            [PgName("Uploaded")]
            Uploaded,

            [PgName("Generated")]
            Generated,

            [PgName("Shared")]
            Shared
        }

        public enum FileCategory
        {
            [PgName("Image")]
            Image,

            [PgName("Cad")]
            Cad,

            [PgName("Bim")]
            Bim,

            [PgName("Pdf")]
            Pdf,

            [PgName("Model")]
            Model,
        }

        private string GetFileName()
        {
            return $"{this.Origin}-{this.Id}{PREFIX}{this.Name}";
        }

        public string GetURI()
        {
            if (this.Directory == null)
            {
                return $"{this.Access.ToString().ToLower()}/{this.GetFileName()}";
            }
            else
            {
                return $"{this.Access.ToString().ToLower()}/{this.Directory}/{this.GetFileName()}";
            }
        }
    }
}
