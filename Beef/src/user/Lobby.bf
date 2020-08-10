using System;
using System.Collections;

namespace ari.user
{
	[Reflect]
	class Lobby
	{
		public uint64 id;

		public String serverIp = null ~ delete _;

		public int32 serverPort;

		public List<List<int64>> teams = null ~ delete _;
	}
}
