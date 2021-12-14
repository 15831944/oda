using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using Amazon.Lambda.Core;
using Rayon.Core;
using Rayon.Core.Components;

// Assembly attribute to enable the Lambda function's JSON input to be converted into a .NET class.
[assembly: LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]

namespace Rayon.Lambda
{
    public class Commit
    {
        public static Component.ComponentTypeEnum[] EXCLUDED_COMPONENTS = new Component.ComponentTypeEnum[] {
            Component.ComponentTypeEnum.Selected,
            Component.ComponentTypeEnum.Active,
        };

        /// <summary>
        /// A simple function that takes a string and does a ToUpper
        /// </summary>
        /// <param name="input"></param>
        /// <param name="context"></param>
        /// <returns></returns>
        public int? FunctionHandler(AppSyncEvent<CommitInput> evt, ILambdaContext context)
        {
            return null;
        }

        public class CommitInput
        {
            private Commit Commit { get; set; }
        }

        public class DiffInput
        {
        }

        internal class FilteredInput
        {
            public FilteredInput()
            {
                this.InsertedElements = new List<Element>();
                this.DeletedElements = new List<Element>();
                this.AddedComponents = new List<Component>();
            }

            private ICollection<Element> InsertedElements { get; set; }
            private ICollection<Element> DeletedElements { get; set; }
            private ICollection<Component> AddedComponents { get; set; }
            private ICollection<Component> RemovedComponents { get; set; }
        }
    }
}
