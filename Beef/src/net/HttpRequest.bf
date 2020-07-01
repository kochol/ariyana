using System;

namespace ari.net
{
	public delegate void OnRequestDoneDelegate(HttpResponse res);

	struct HttpRequest
	{
		public String Url;
		public OnRequestDoneDelegate OnRequestDone;
	}
}
