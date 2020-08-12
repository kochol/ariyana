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

	public delegate void dOnGameList(GameList games);

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
				Login(response.Body);
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

		public void Login(String _token)
		{
			let token = new String();
			token.Append("Authorization: ", "Bearer ", _token);
			headers.Add(token);

			let ct = new String();
			ct.Append("Content-Type: application/json; charset=utf-8");
			headers.Add(ct);

			http_client.session.SetHeaders(headers);
			isLoggedIn = true;
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
			// clear the last headers
			ClearAndDeleteItems(headers);
			http_client.session.SetHeaders(null);

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
				Player player = new Player();
				var r = JSON_Beef.Serialization.JSONDeserializer.Deserialize<Player>(res.Body, player);
				switch (r)
				{
				case .Err(let err):
					Console.WriteLine(err);
					delete player;
				case .Ok:
					if (OnPlayerData != null)
					{
						OnPlayerData(player);
					}
					else
					{
						delete player;
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
				Lobby lobby = new Lobby();
				var r = JSON_Beef.Serialization.JSONDeserializer.Deserialize<Lobby>(res.Body, lobby);
				switch (r)
				{
				case .Err(let err):
					Console.WriteLine(err);
					delete lobby;
				case .Ok:
					if (OnJoinedLobby != null)
					{
						OnJoinedLobby(lobby);
					}
					else
					{
						delete lobby;
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

		public void GetGames(int32 offset, int32 count, dOnGameList onGameList, dOnHttpFailed onFailed)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("player/games/{}/{}", offset, count);
			req.OnRequestDone = new (res) => {
				if (res.StatusCode == 200)
				{
					var games = new GameList();
					JSON_Beef.Serialization.JSONDeserializer.Deserialize<List<Game>>(res.Body, games.Games);
					if (onGameList != null)
						onGameList(games);
					else
						delete games;
				}
				else
				{
					let err = scope String();
					err.AppendF("Can not get player games {} {}", res.Status, res.StatusCode);
					Logger.Error(err);
					if (onFailed != null)
						onFailed(res.Status);
				}
				res.Dispose();
			};
			http_client.AddRequest(ref req);
		}

		public void ServerStartGame(int64 lobby_id)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("server/game_start/{}", lobby_id);
			http_client.AddRequest(ref req);
		}

		public void ServerGetLobby(int64 lobby_id, dOnJoinedLobby _OnGetLobby)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("server/get_lobby/{}", lobby_id);
			req.OnRequestDone = new (res) => {
				if (res.StatusCode == 200)
				{
					Lobby lobby = new Lobby();
					var r = JSON_Beef.Serialization.JSONDeserializer.Deserialize<Lobby>(res.Body, lobby);
					switch (r)
					{
					case .Err(let err):
						Console.WriteLine(err);
						delete lobby;
					case .Ok:
						if (_OnGetLobby != null)
						{
							_OnGetLobby(lobby);
							delete _OnGetLobby;
						}
						else
						{
							delete lobby;
						}
					}
				}
				res.Dispose();
			};
			http_client.AddRequest(ref req);
		}

		public void ServerUploadReplay(int64 game_id, uint8* data, int32 size, OnRequestDoneDelegate OnDone)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.AppendF("server/save_replay/{}", game_id);
			req.FileData = data;
			req.FileSize = size;
			req.Verb = .Put;
			req.OnRequestDone = OnDone;
			http_client.AddRequest(ref req);
		}

		public void ServerSaveGame(Game game, OnRequestDoneDelegate OnDone)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.Append("server/save_game");
			for (var t in game.teams)
			{
				for (var p in t)
				{
					p.score.Replace('\"', '\'');
				}
			}
			let r = JSON_Beef.Serialization.JSONSerializer.Serialize<String>(game);
			req.Verb = .Post;
			if (r case .Ok(let val))
			{
				req.Body = val;
			}
			req.OnRequestDone = OnDone;
			http_client.AddRequest(ref req);
		}

		public void DownloadReplay(int64 game_id, OnRequestDoneDelegate OnDone)
		{
			HttpRequest req = .();
			req.Url = new String(ServerAddress);
			req.Url.RemoveFromEnd(4);
			req.Url.AppendF("replays/{}.zip", game_id);
			req.OnRequestDone = OnDone;
			http_client.AddRequest(ref req);
		}
	}
}
