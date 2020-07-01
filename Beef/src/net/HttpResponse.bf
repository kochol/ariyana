using System;

namespace ari.net
{
	struct HttpResponse
	{
		public String Body;
		public curl.Easy.ReturnCode Status;
		public int64 StatusCode;

		public void Dispose()
		{
			delete Body;
		}
	}
}
