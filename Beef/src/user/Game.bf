using System.Collections;
using System;

namespace ari.user
{
	[Reflect, AlwaysInclude(AssumeInstantiated=true, IncludeAllMethods=true)]
	class Game
	{
		public int64 id;

		public List<List<PlayerScore>> teams = null ~ {
			if (_ != null)
			{
				for (var value in _)
					DeleteContainerAndItems!(value);
				delete _;
			}
		};

		public int32 winnerTeamId;

		public String playTime = null ~ delete _;

		public String version = null ~ delete _;

		public double gameDuration;
	}
}
