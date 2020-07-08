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

	public delegate void dOnJoinedLobby(Lobby lobby);

	public class ProfileSystem: AriSystem
	{
		private String ServerAddress = null ~ delete _;
		private HttpClientService http_client;
		List<String> headers = new List<String>() ~ DeleteContainerAndItems!(_);
		bool isLoggedIn = false;
		float sendAutoJoinAgain = -1;


		// Callbacks
		public dOnLoggedIn OnLoggedIn = null ~ delete _;
		public dOnHttpFailed OnLoginFailed = null ~ delete _;
		public dOnLoggedIn OnRegistered = null ~ delete _;

		public dOnPlayerData OnPlayerData = null ~ delete _;
		public dOnHttpFailed OnHttpFaild = null ~ delete _;

		public dOnJoinedLobby OnJoinedLobby = null ~ delete _;

		// Constructor
		public this(String server_address, HttpClientService _http_client)
		{
			ServerAddress = new String(server_address);
			http_client = _http_client;
		}

		protected override void Update(World _world, float _elapsed)
		{
			base.Update(_world, _elapsed);
			if (sendAutoJoinAgain > 0)
			{
				sendAutoJoinAgain -= _elapsed;
				if (sendAutoJoinAgain <= 0)
					AutoJoinToLobby();
			}
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

		public void Login(String username, String password)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("auth/{}/{}", username, password);
			req.OnRequestDone = new => OnLogin;
			http_client.AddRequest(ref req);
		}

		public bool IsLoggedIn()
		{
			return isLoggedIn;
		}

		void OnGetPlayerDataCB(HttpResponse res)
		{
			if (res.Status == .Ok && res.StatusCode == 200)
			{
				var r = JSON_Beef.JSONDeserializer.Deserialize<Player>(res.Body);
				switch (r)
				{
				case .Err(let err):
					Console.WriteLine(err);
				case .Ok(let val):
					if (OnPlayerData != null)
					{
						OnPlayerData(val);
					}
					else
					{
						delete val;
					}
				}
			}
			else
			{
				Console.WriteLine("{} {}", res.Status, res.StatusCode);
				if (OnHttpFaild != null)
					OnHttpFaild(res.Status);
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

		void OnAutoJoinToLobbyCB(HttpResponse res)
		{
			if (res.StatusCode == 200)
			{
				// We found a lobby to join
				var r = JSON_Beef.JSONDeserializer.Deserialize<Lobby>(res.Body);
				switch (r)
				{
				case .Err(let err):
					Console.WriteLine(err);
				case .Ok(let val):
					if (OnJoinedLobby != null)
					{
						OnJoinedLobby(val);
					}
					else
					{
						delete val;
					}
				}
			}
			else if(res.StatusCode == 204)
			{
				// we have to check again in next 3 seconds
				sendAutoJoinAgain = 3;
			}
			else
			{
				Console.WriteLine("{} {}", res.Status, res.StatusCode);
				if (OnHttpFaild != null)
					OnHttpFaild(res.Status);
			}
			res.Dispose();
		}

		public void AutoJoinToLobby()
		{
			sendAutoJoinAgain = -1;
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.Append("player/lobby");
			req.OnRequestDone = new => OnAutoJoinToLobbyCB;
			http_client.AddRequest(ref req);
		}

		public void CancelAutoJoinToLobby()
		{
			sendAutoJoinAgain = -2;
		}

		void OnRegisterCB(HttpResponse res)
		{
			if (res.StatusCode == 200)
			{
				if (OnRegistered != null)
					OnRegistered();
			}
			else
			{
				Console.WriteLine("{} {}", res.Status, res.StatusCode);
				if (OnHttpFaild != null)
					OnHttpFaild(res.Status);
			}
			res.Dispose();
		}

		public void Register(String userName, String password)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("auth/register/{}/{}/{}", Io.GetDeviceID(), userName, password);
			req.OnRequestDone = new => OnRegisterCB;
			http_client.AddRequest(ref req);
		}
	}
}
