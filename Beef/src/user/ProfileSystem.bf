using System;
using System.Threading;
using ari.net;
using curl;

namespace ari.user
{
	public delegate void dOnLoggedIn();
	public delegate void dOnLoginFailed(Easy.ReturnCode err);

	public class ProfileSystem
	{
		private String ServerAddress = null ~ delete _;
		private String Token = new String() ~ delete _;
		private HttpClientService http_client;

		// Callbacks
		public dOnLoggedIn OnLoggedIn = null ~ delete _;
		public dOnLoginFailed OnLoginFailed = null ~ delete _;

		public this(String server_address, HttpClientService _http_client)
		{
			ServerAddress = new String(server_address);
			http_client = _http_client;
		}

		void OnLogin(HttpResponse response)
		{
			if (response.Status == .Ok)
			{
				Token.Append("Bearer ", response.Body);
				response.Dispose();
				OnLoggedIn();
			}
			else
			{
				Console.WriteLine(response.Status);
				if (OnLoginFailed != null)
					OnLoginFailed(response.Status);
			}
		}

		public void Login()
		{
			HttpRequest login = .();
			login.Url = new String(ServerAddress);
			login.Url.AppendF("auth/{}/", Io.GetDeviceID());
#if BF_PLATFORM_WINDOWS
			login.Url.Append("Windows/");
#elif BF_PLATFORM_LINUX
			login.Url.Append("Linux/");
#elif BF_PLATFORM_ANDROID
			login.Url.Append("Android/");
#endif
			let os = scope OperatingSystem();
			os.ToString(login.Url);

			login.OnRequestDone = new => OnLogin;

			http_client.AddRequest(ref login);
		}

		public bool IsLoggedIn()
		{
			return Token.Length > 0;
		}
	}
}
