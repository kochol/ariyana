using System;

namespace ari.net
{
	struct HttpResponse
	{
		public String Body;
		public curl.Easy.ReturnCode Status;

		public void Dispose()
		{
			delete Body;
		}
	}
}
