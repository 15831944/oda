// <copyright file="Model.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Text.Json;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components;

    public class Model
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Model"/> class.
        /// </summary>
        public Model()
        {
            this.Id = Guid.NewGuid();
            this.Elements = new List<Element>();
            this.Commits = new List<Commit>();
            this.Components = new List<Component>();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Model"/> class.
        /// </summary>
        /// <param name="name"></param>
        /// <param name="owner"></param>
        /// <param name="folderId"></param>
        public Model(string name, User owner, Guid? folderId)
        {
            this.Id = Guid.NewGuid();
            this.Name = name;
            this.Elements = new List<Element>();
            this.Commits = new List<Commit>();
            this.Components = new List<Component>();

            this.OwnerId = owner.Id;
            this.Owner = owner;
            this.FolderId = folderId;
        }

        public Guid Id { get; set; }

        public string Name { get; set; }

        public DateTime CreatedAt { get; set; }

        public DateTime UpdatedAt { get; set; }

        public Guid OwnerId { get; set; }

        public virtual User Owner { get; set; }

        public Guid? FolderId { get; set; }

        public virtual Folder Folder { get; set; }

        public virtual ICollection<Element> Elements { get; set; }

        public virtual ICollection<Commit> Commits { get; set; }

        /// <summary>
        /// Used for flattening structure in bulk copy insert
        /// </summary>
        public ICollection<Component> Components { get; set; }

        public void SetSettings(ModelSettingsComp.UnitSystemEnum unit)
        {
            var element = new Element(this)
                .With(new ModelSettingsComp(unit));
            this.AddElement(element);
        }

        public void AddElement(Element element)
        {
            if (element == null)
            {
                return;
            }

            element.ModelId = this.Id;
            element.Model = this;

            element.CreatedById = this.OwnerId;
            element.CreatedBy = this.Owner;
            foreach (Components.Component component in element.Components)
            {
                component.ModifiedBy = this.Owner;
                component.ModifiedById = this.OwnerId;
            }

            this.Elements.Add(element);
        }

        public void AddCommit(Commit commit)
        {
            this.Commits.Add(commit);
        }

        /// <summary>
        /// Returns a string mocking the result of the graphql api GetElements
        /// </summary>
        /// <returns></returns>
        public void MockGetElements(string filePath, bool withHeader)
        {
            JsonSerializerOptions options = new JsonSerializerOptions
            {
                IgnoreNullValues = true,
            };

            using StreamWriter sw = new StreamWriter(filePath);
            if (withHeader)
            {
                sw.Write("{\"data\":{\"getElements\":");
            }

            sw.Write("[");
            var i = 0;
            foreach (Element element in this.Elements)
            {
                if (i > 0)
                {
                    sw.Write(",");
                }

                sw.Write(JsonSerializer.Serialize(element, options));
                i++;
            }

            sw.Write("]");
            if (withHeader)
            {
                sw.Write("}}");
            }
        }
    }
}
