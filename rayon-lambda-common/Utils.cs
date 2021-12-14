using System;

namespace Rayon.Lambda
{
    /// <summary>
    /// An AppSyncEvent Containing A T Payload
    /// </summary>
    public class AppSyncEvent<T>
    {
        public AppSyncEvent()
        {
        }

        public AppSyncEvent(string requestType, T file)
        {
            this.RequestType = requestType;
            this.Arguments = file;
        }

        public AppSyncEventIdentity Identity { get; set; }
        public string RequestType { set; get; }
        public T Arguments { set; get; }
    }

    public class AppSyncEventIdentity
    {
        public Guid Sub { get; set; }
    }
}
