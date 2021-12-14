using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Text.Json.Serialization;

using Amazon.Lambda.Core;
using Amazon.S3;

using Rhino.FileIO;

using Rayon.Lambda;

using Ry = Rayon.Core;

// Assembly attribute to enable the Lambda function's JSON input to be converted into a .NET class.
[assembly: LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]

namespace Rayon.Rhino
{
    public class Function
    {
        private IAmazonS3 S3Client { get; }
        private string S3BucketName { get; }

        /// <summary>
        /// </summary>
        public Function()
        {
            string region = Environment.GetEnvironmentVariable("RAYON_REGION");
            Amazon.RegionEndpoint endpoint = Amazon.RegionEndpoint.GetBySystemName(region);
            this.S3Client = new AmazonS3Client(endpoint);
            this.S3BucketName = Environment.GetEnvironmentVariable("RAYON_USER_FILE_BUCKET");
        }

        /// <summary>
        /// Constructor used for testing which will pass in the already configured service clients.
        /// </summary>
        /// <param name="s3Client"></param>
        public Function(IAmazonS3 s3Client)
        {
            this.S3Client = s3Client;
            this.S3BucketName = Environment.GetEnvironmentVariable("RAYON_USER_FILE_BUCKET");
        }

        /// <summary>
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public async Task<FunctionOutput> FunctionHandler(AppSyncEvent<FunctionInput> input)
        {
            if (input.Identity?.Sub == null)
            {
                throw new UnauthorizedAccessException("User must be authenticated");
            }

            using var db = new Ry.DatabaseContext();

            // Fetching user
            LambdaLogger.Log($"Fetching user from DB: {input.Identity.Sub}");
            var owner = db.Users.Single(b => b.Id == input.Identity.Sub);
            if (owner == null)
            {
                throw new UnauthorizedAccessException("Invalid User");
            }

            // Fetching File
            LambdaLogger.Log($"Fetching file from DB: {input.Arguments}");
            var rayonFile = db.Files.Single(f => f.OwnerId == input.Identity.Sub && f.Id == input.Arguments.FileId);
            if (rayonFile == null)
            {
                throw new UnauthorizedAccessException("Not Authorized to Access the File");
            }

            // Starting the import
            LambdaLogger.Log($"Starting Rhino Import: {rayonFile.Name}");
            var uri = rayonFile.GetURI();

            LambdaLogger.Log($"Fetching file on s3: {uri}");
            var response = await this.S3Client.GetObjectAsync(this.S3BucketName, uri);

            using var responseStream = response.ResponseStream;
            using MemoryStream ms = new MemoryStream();
            responseStream.CopyTo(ms);

            LambdaLogger.Log($"Copied S3 stream");

            File3dm file = File3dm.FromByteArray(ms.ToArray());

            var model = new Ry.Model(rayonFile.Name, owner, rayonFile.FolderId);

            // we create a commit with no diffs (to prevent DB overload)
            // Per convention external file import are not undoable/redoable
            var commit = new Ry.Commit($"Import {rayonFile.Name}", owner, model);
            db.Models.Add(model);
            db.Commits.Add(commit);
            LambdaLogger.Log($"Saving Model to DB");
            await db.SaveChangesAsync();
            LambdaLogger.Log($"Saved Model to DB");

            var rawDbContext = new Ry.RawDatabaseContext();
            using var connection = rawDbContext.Connection();
            connection.Open();

            // Note : we are not using a transaction for faster insert
            //        but we rollback the change in case of error by deleting the project
            //        which will trigger the removal of all elements and components
            try
            {
                LambdaLogger.Log($"Converting Rhino model");
                var importedModel = this.ImportRhinoFile(model, file);

                LambdaLogger.Log($"Saving Rhino elements to DB");
                rawDbContext.InsertElements(importedModel.Elements, connection);

                LambdaLogger.Log($"Saving Rhino components to DB");
                rawDbContext.InsertComponents(importedModel.Components, connection);

                connection.Close();

                var output = new FunctionOutput(importedModel);
                return output;
            }
            catch (Exception err)
            {
                connection.Close();

                // Remove the failed model
                LambdaLogger.Log($"Error while saving model: {err.Message}");
                db.Models.Remove(model);
                await db.SaveChangesAsync();
                throw err;
            }
        }

        public Ry.Model ImportRhinoFile(Ry.Model model, File3dm file)
        {
            RhinoImporter importer = new RhinoImporter(model, file);

            LambdaLogger.Log($"Converting Rhino model");
            importer.ImportAllLayers();
            importer.ImportAllGeometryObjects();

            return importer.Model;
        }
    }

    public class FunctionOutput
    {
        public FunctionOutput(Ry.Model model)
        {
            this.Id = model.Id;
            this.Name = model.Name;
            this.Owner = model.Owner;
            this.CreatedAt = model.CreatedAt.ToString();
            this.UpdatedAt = model.UpdatedAt.ToString();
        }

        [JsonPropertyName("id")]
        public Guid Id { get; set; }

        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("owner")]
        public Ry.User Owner { get; set; }

        [JsonPropertyName("createdAt")]
        public string CreatedAt { get; set; }

        [JsonPropertyName("updatedAt")]
        public string UpdatedAt { get; set; }
    }

    public class FunctionInput
    {
        public FunctionInput(Guid fileId)
        {
            this.FileId = fileId;
        }

        public Guid FileId { get; set; }
    }
}
