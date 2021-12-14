using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Amazon.Lambda.APIGatewayEvents;
using Amazon.Lambda;
using System.Text.Json;
using System.Text;
using System.Text.Json.Serialization;
using System.IO.Compression;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

using Amazon.Lambda.Core;
using Rayon.Core;
using Npgsql;

using Amazon.S3;
using Amazon.S3.Model;

// Assembly attribute to enable the Lambda function's JSON input to be converted into a .NET class.
[assembly: LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]

namespace Rayon.GetElements
{
    public class Function
    {
        private static int MAX_DIRECT_RESPONSE = 5000;

        // We create a list with an initial capacity of 16 components
        // totally arbitrary value in order to prevent the reallocation when the List
        // capacity increases from 2 to 4 to 8 to 16
        private static int INITIAL_COMPONENT_LIST_CAPACITY = 16;

        public static byte[] Compress(byte[] bytes)
        {
            using (var mso = new MemoryStream())
            {
                using (var gs = new GZipStream(mso, CompressionMode.Compress))
                {
                    gs.Write(bytes, 0, bytes.Length);
                }
                byte[] encodedbytes = mso.ToArray();
                return encodedbytes;
            }
        }

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
        /// Returns the list of elements of the desired model.
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        public async Task<APIGatewayProxyResponse> FunctionHandler(APIGatewayProxyRequest request)
        {
            LambdaLogger.Log($"receiving request: {JsonSerializer.Serialize(request)}");
            var response = new APIGatewayProxyResponse();

            // retrieve query path parameters
            string token = null;
            string modelId = null;
            string userId = null;

            // QueryStringParameters : category

            if (request.PathParameters.ContainsKey("token"))
            {
                token = request.PathParameters?["token"];
            }

            if (request.PathParameters.ContainsKey("id"))
            {
                modelId = request.PathParameters?["id"];
            }

            if (request.RequestContext.Authorizer?.Claims.ContainsKey("sub") ?? false)
            {
                userId = request.RequestContext.Authorizer?.Claims?["sub"];
            }

            LambdaLogger.Log($"Getting connection: {DateTime.Now}");
            var db = new RawDatabaseContext();
            using var conn = db.Connection();
            conn.Open();

            Guid? modelGuid;

            LambdaLogger.Log($"Checking authorization: {DateTime.Now}");
            if (token != null)
            {
                // token authentication
                modelGuid = this.GetModelIdFromToken(token, conn);
                if (modelGuid == null)
                {
                    return ReturnUnauthorizedError(response);
                }
            }
            else
            {
                // modelId authentication
                if (modelId == null || userId == null || !this.IsAuthorized(Guid.Parse(modelId), Guid.Parse(userId), conn))
                {
                    return ReturnUnauthorizedError(response);
                }
                modelGuid = Guid.Parse(modelId);
            }

            LambdaLogger.Log($"Retrieving raw data: {DateTime.Now}");
            Dictionary<string, ElementOutput> elementsMap = new Dictionary<string, ElementOutput>();

            using (var cmd = new NpgsqlCommand("SELECT component.handle, component.component_type, component.value FROM component WHERE component.model_id = @id", conn))
            {
                cmd.Parameters.AddWithValue("id", modelGuid);
                using (var reader = cmd.ExecuteReader())
                {
                    LambdaLogger.Log($"Formatting raw data: {DateTime.Now}");
                    while (reader.Read())
                    {
                        var handle = reader.GetString(0);
                        var componentType = reader.GetInt32(1);
                        var value = reader.GetString(2);

                        ComponentOutput newComponentOutput = new ComponentOutput
                        {
                            ComponentType = componentType,
                            Value = value
                        };

                        if (elementsMap.ContainsKey(handle))
                        {
                            elementsMap[handle].Components.Add(newComponentOutput);
                        }
                        else
                        {
                            elementsMap.Add(handle, new ElementOutput
                            {
                                Handle = handle,
                                Components = new List<ComponentOutput>(INITIAL_COMPONENT_LIST_CAPACITY) { newComponentOutput, }
                            });
                        }
                    }
                }
            }

            // retrieve the current version of the model
            int? version = 0;
            if (modelGuid is Guid id)
            {
                version = this.GetCurrentModelVersion(id, conn);
            }

            conn.Close();

            LambdaLogger.Log($"Returning elements: {DateTime.Now}");
            var output = new FunctionOutput
            {
                ModelId = (Guid)modelGuid,
                Version = version ?? 0,
            };

            // for huge models we need to store them in s3
            if (elementsMap.Count > MAX_DIRECT_RESPONSE)
            {
                string s3key = $"temp/{modelId}_{Guid.NewGuid()}.json";

                LambdaLogger.Log($"Serializing data: {DateTime.Now}");
                string jsonString = JsonSerializer.Serialize(elementsMap.Values);
                byte[] bytes = Encoding.UTF8.GetBytes(jsonString);

                LambdaLogger.Log($"Sending data: {DateTime.Now}");

                // TODO : add an expiry date
                PutObjectRequest putRequest = new PutObjectRequest
                {
                    BucketName = this.S3BucketName,
                    Key = s3key,
                    InputStream = new MemoryStream(Function.Compress(bytes)),
                    ContentType = "application/json",
                };

                putRequest.Headers.ContentEncoding = "gzip";

                await this.S3Client.PutObjectAsync(putRequest);

                LambdaLogger.Log($"Returning uri: {DateTime.Now}");
                output.Uri = s3key;
            }
            else
            {
                output.Data = elementsMap.Values.ToList();
            }

            response.StatusCode = 200;
            response.Body = JsonSerializer.Serialize(output);
            response.Headers = new Dictionary<string, string>
            {
                { "Access-Control-Allow-Origin", "*" },
                { "Access-Control-Allow-Credentials", "true" }
            };

            return response;
        }

        private static APIGatewayProxyResponse ReturnUnauthorizedError(APIGatewayProxyResponse response)
        {
            LambdaLogger.Log($"Unauthorized: {DateTime.Now}");
            response.Headers = new Dictionary<string, string>
                    {
                        { "Access-Control-Allow-Origin", "*" },
                        { "Access-Control-Allow-Credentials", "true" }
                    };
            response.StatusCode = 401;
            response.Body = "Unauthorized";
            return response;
        }

        public bool IsAuthorized(Guid modelId, Guid userId, NpgsqlConnection conn)
        {
            using (var cmd = new NpgsqlCommand("SELECT 1 FROM model WHERE model.id = @id and model.owner_id = @ownerId", conn))
            {
                cmd.Parameters.AddWithValue("id", modelId);
                cmd.Parameters.AddWithValue("ownerId", userId);
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        return true;
                    }

                    return false;
                }
            }
        }

        public Guid? GetModelIdFromToken(string token, NpgsqlConnection conn)
        {
            using (var cmd = new NpgsqlCommand("SELECT model.id FROM model WHERE model.token = @token and model.token_is_active is TRUE", conn))
            {
                cmd.Parameters.AddWithValue("token", token);
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        return reader.GetGuid(0);
                    }

                    return null;
                }
            }
        }

        public int? GetCurrentModelVersion(Guid modelId, NpgsqlConnection conn)
        {
            using (var cmd = new NpgsqlCommand("SELECT MAX(commit.id) FROM commit WHERE commit.model_id = @id", conn))
            {
                cmd.Parameters.AddWithValue("id", modelId);
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        if (reader.IsDBNull(0))
                        {
                            return null;
                        }
                        else
                        {
                            return reader.GetInt32(0);
                        }
                    }

                    return null;
                }
            }
        }
    }

    public class FunctionOutput
    {
        [JsonPropertyName("uri")]
        public string Uri { get; set; }

        [JsonPropertyName("modelId")]
        public Guid ModelId { get; set; }

        [JsonPropertyName("data")]
        public ICollection<ElementOutput> Data { get; set; }

        [JsonPropertyName("version")]
        public int Version { get; set; }
    }

    public class ElementOutput
    {
        [JsonPropertyName("h")]
        public string Handle { get; set; }

        [JsonPropertyName("c")]
        public ICollection<ComponentOutput> Components { get; set; }
    }

    public class ComponentOutput
    {
        [JsonPropertyName("t")]
        public int ComponentType { get; set; }

        [JsonPropertyName("v")]
        public string Value { get; set; }
    }
}
