using System;

namespace ari.user
{
	[Reflect]
	public class Player
	{
		public uint64 id;

		public String userName = null ~ delete _;

		public String password = null ~ delete _;

		public String registerDate = null ~ delete _;

		public String lastLogin = null ~ delete _;

		public String email = null ~ delete _;

		public int32 accountStatus;

		public int32 mmr;
	}
}
