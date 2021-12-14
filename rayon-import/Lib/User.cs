using System;
using System.Collections.Generic;
using System.Text;
using RayonImport.Lib.Components;

using System.Text.Json.Serialization;

namespace RayonImport.Lib
{
    public class User
    {
        public User()
        {
            this.Id = Guid.NewGuid();
        }

        public User(Guid id)
        {
            this.Id = id;
        }

        [JsonPropertyName("id")]
        public Guid Id { get; set; }

        [JsonPropertyName("username")]
        public string Username { get; set; }

        [JsonPropertyName("email")]
        public string Email { get; set; }
    }
}
