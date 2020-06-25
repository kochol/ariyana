using System;

namespace ari.user
{
	[Reflect]
	public class Player
	{
		public uint64 id;

		public String userName = null ~ delete _;

		public String password = null ~ delete _;
	}
}
