using System;
using System.Linq;
using System.Threading.Tasks;

using Xunit;
using Amazon.Lambda.Core;
using Amazon.Lambda.TestUtilities;

using Amazon;
using Amazon.S3;

using Ry = Rayon.Core;
using Rhino.FileIO;
using System.Text.Json;
using Rayon.Lambda;

namespace Rayon.Rhino.Tests
{
    public class FunctionTest
    {
        [Fact]
        public async Task IntegrationTest()
        {
            IAmazonS3 s3Client = new AmazonS3Client(RegionEndpoint.EUWest1);

            var bucketName = "lambda-lambda-rhino-import-".ToLower() + DateTime.Now.Ticks;
            using var db = new Ry.DatabaseContext();
            Ry.File file = new Ry.File
            {
                Origin = Ry.File.FileOrigin.Uploaded,
                Access = Ry.File.FileAccess.Public,
                Name = "myfile.3dm",
                Id = Guid.NewGuid(),
                Category = Ry.File.FileCategory.Cad,
                Hidden = false,
                Size = 2048
            };

            try
            {
                // Use test constructor for the function with the service clients created for the test
                var function = new Function(s3Client);

                var context = new TestLambdaContext();

                // Add the file to the db
                db.Files.Add(file);

                var input = new AppSyncEvent<FunctionInput>("importModel", new FunctionInput(file.Id));

                // The user is not authorized
                await Assert.ThrowsAsync<UnauthorizedAccessException>(async () => await function.FunctionHandler(input));
            }
            finally
            {
                // Clean up the test data
                db.Files.Remove(file);
            }
        }

        [Fact]
        public async Task TestDBSave()
        {
            string filePath = @"/Users/basti/rayon/resources/3dm/test.3dm";
            using var db = new Ry.DatabaseContext();
            LambdaLogger.Log($"Fetching user from DB");
            var owner = db.Users.Single(b => b.Username == "bast");
            LambdaLogger.Log($"Fetched user from DB");
            if (owner == null)
            {
                throw new UnauthorizedAccessException("Invalid User");
            }

            File3dm file = File3dm.Read(filePath);

            var model = new Ry.Model("test", owner, null);

            // we create a commit with no diffs (to prevent DB overload)
            // Per convention external file import are not undoable/redoable
            var commit = new Ry.Commit($"Import file", owner, model);
            db.Models.Add(model);
            db.Commits.Add(commit);
            LambdaLogger.Log($"Saving User to DB");
            await db.SaveChangesAsync();
            LambdaLogger.Log($"Saved User to DB");

            var function = new Function();
            LambdaLogger.Log($"Converting Rhino model");
            var importedModel = function.ImportRhinoFile(model, file);

            LambdaLogger.Log($"Saving Rhino model to DB");

            var rawDbContext = new Ry.RawDatabaseContext();
            var connection = rawDbContext.Connection();
            connection.Open();
            var transaction = connection.BeginTransaction();

            try
            {
                rawDbContext.InsertElements(importedModel.Elements, connection);
                rawDbContext.InsertComponents(importedModel.Components, connection);
                var output = new FunctionOutput(importedModel);
                LambdaLogger.Log($"Returning: {JsonSerializer.Serialize(output)}");

                transaction.Commit();
                connection.Close();
            }
            catch (Exception ex)
            {
                // Cancel transaction
                if (transaction != null)
                {
                    transaction.Rollback();
                }
                LambdaLogger.Log($"Error while saving model to the db: {ex.Message}");
                connection.Close();
            }
            finally
            {
                // cleanup and delete model
                //db.Models.Remove(model);
                //await db.SaveChangesAsync();
            }
        }
    }
}
