// -----------------------------------------------------------------------
// <copyright file="ApiClient.cs" company="Implbits Software LLC">
// All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

namespace TetraSlide.Api.Clients
{
    using System;
    using System.Net.Http;
    using System.Net.Http.Headers;

    /// <summary>
    /// Base class for Tetraslide API Client.  Encapsulates discovery/setting of the 
    /// base TetraSlide API endpoint and possibly other common functionality
    /// </summary>
    public abstract class ApiClient
    {
        /// <summary>
        /// Initializes a new instance of the ApiClient class. Uses discovered
        /// (or possibly temporarily hard-coded) location for TetraSlide API service
        /// </summary>
        /// <param name="accessToken">Access Token used to when calling the TetraSlide APi</param>
        public ApiClient(string accessToken)
        {
            this.Init(accessToken, "https://services.implbits.com/tetraslide/v1/api/");
        }

        /// <summary>
        /// Initializes a new instance of the ApiClient class. Uses discovered
        /// (or possibly temporarily hard-coded) location for TetraSlide API service        
        /// </summary>
        /// <param name="accessToken">Access Token used to when calling the TetraSlide APi</param>
        /// <param name="baseUrl">Base url of the Tetraslide API</param>
        public ApiClient(string accessToken, string baseUrl)
        {
            this.Init(accessToken, baseUrl);
        }

        /// <summary>
        /// Gets the Access Token used to communicate with API
        /// </summary>
        public string AccessToken
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the HttpClient used to communicate with API
        /// </summary>
        public HttpClient Client
        {
            get;
            private set;
        } 

        /// <summary>
        /// Initialize the HttpClient
        /// </summary>
        /// <param name="accessToken">Access Token used to when calling the TetraSlide APi</param>
        /// <param name="baseUrl">Base url of the Tetraslide API</param>
        private void Init(string accessToken, string baseUrl)
        {
            this.AccessToken = accessToken;
            this.Client = new HttpClient();
            this.Client.BaseAddress = new Uri(baseUrl);
            this.Client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        }
    }
}
