using System;
using System.Threading;
using ari.net;
using curl;
using System.Collections;

namespace ari.user
{
	public delegate void dOnLoggedIn();
	public delegate void dOnHttpFailed(Easy.ReturnCode err);

	public delegate void dOnPlayerData(Player player);

	public class ProfileSystem
	{
		private String ServerAddress = null ~ delete _;
		private HttpClientService http_client;
		List<String> headers = new List<String>() ~ DeleteContainerAndItems!(_);
		bool isLoggedIn = false;

		// Callbacks
		public dOnLoggedIn OnLoggedIn = null ~ delete _;
		public dOnHttpFailed OnLoginFailed = null ~ delete _;

		public dOnPlayerData OnPlayerData = null ~ delete _;

		// Constructor
		public this(String server_address, HttpClientService _http_client)
		{
			ServerAddress = new String(server_address);
			http_client = _http_client;
		}

		void OnLogin(HttpResponse response)
		{
			if (response.Status == .Ok && response.StatusCode == 200)
			{
				var token = new String();
				token.Append("Authorization: ", "Bearer ", response.Body);
				headers.Add(token);
				http_client.session.SetHeaders(headers);
				isLoggedIn = true;
				OnLoggedIn();
			}
			else
			{
				Console.WriteLine("{} {}", response.Status, response.StatusCode);
				if (OnLoginFailed != null)
					OnLoginFailed(response.Status);
			}
			response.Dispose();
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
			return isLoggedIn;
		}

		void OnGetPlayerDataCB(HttpResponse res)
		{
			if (res.Status == .Ok && res.StatusCode == 200)
			{
				Console.WriteLine(res.Body);
			}
			else
			{
				Console.WriteLine("{} {}", res.Status, res.StatusCode);
			}
			res.Dispose();
		}

		public void GetPlayerData()
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.Append("player");
			req.OnRequestDone = new => OnGetPlayerDataCB;
			http_client.AddRequest(ref req);
		}
	}
}
