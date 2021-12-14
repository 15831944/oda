using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using Xunit;
using Amazon.Lambda.APIGatewayEvents;
using Amazon.Lambda.TestUtilities;

namespace Rayon.GetElements.Tests
{
    public class FunctionTest
    {
        [Fact]
        public async void TestCompressData()
        {
            // Invoke the lambda function
            var function = new Function();
            var input = new APIGatewayProxyRequest();
            input.PathParameters = new Dictionary<string, string>();
            input.PathParameters.Add("id", "d769cfd5-3468-47aa-a093-81f0a2af6891");
            input.RequestContext = new APIGatewayProxyRequest.ProxyRequestContext();
            input.RequestContext.Authorizer = new APIGatewayCustomAuthorizerContext();
            input.RequestContext.Authorizer.Claims = new Dictionary<string, string>();
            input.RequestContext.Authorizer.Claims["sub"] = "d952802c-c84f-41cb-92f8-036e0f0d943b";

            var output = await function.FunctionHandler(input);
        }
    }
}
