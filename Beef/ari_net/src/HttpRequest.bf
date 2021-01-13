using System;
using System.Collections;

namespace ari.net
{
	public delegate void OnRequestDoneDelegate(HttpResponse res);

	struct HttpRequest
	{
		public String Url;
		List<String> Headers;
		bool SetHeaders;
		public OnRequestDoneDelegate OnRequestDone;

		// File upload with put
		public uint8* FileData;
		public int32 FileSize;

		// Set the request body
		public String Body;

		public curl.Session.Verbs Verb = .Get;

		public void SetHeaders(List<String> _headers) mut
		{
			Headers = _headers;
			SetHeaders = true;
		}

		public void Dispose()
		{
			delete Url;
			DeleteContainerAndItems!(Headers);
			delete Body;
		}
	}
}
