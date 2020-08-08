using System;

namespace ari.user
{
	[Reflect]
	class PlayerScore
	{
		public int64 playerId;

		public String score = null ~ delete _;
	}
}
