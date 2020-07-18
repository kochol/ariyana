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

		public void SetHeaders(List<String> _headers) mut
		{
			Headers = _headers;
			SetHeaders = true;
		}

		public void Dispose()
		{
			delete Url;
			DeleteContainerAndItems!(Headers);
		}
	}
}
