using System;

namespace ari
{
	public static class Timer
	{
		[LinkName("ari_tm_init")]
		public static extern void Init();

		[LinkName("ari_tm_now")]
		public static extern uint64 Now();

		[LinkName("ari_tm_diff")]
		public static extern uint64 Diff(uint64 new_ticks, uint64 old_ticks);

		[LinkName("ari_tm_since")]
		public static extern uint64 Since(uint64 start_ticks);

		[LinkName("ari_tm_laptime")]
		public static extern uint64 LapTime(ref uint64 last_ticks);

		[LinkName("ari_tm_sec")]
		public static extern double ToSeconds(uint64 ticks);

		[LinkName("ari_tm_ms")]
		public static extern double ToMilliSeconds(uint64 ticks);

		[LinkName("ari_tm_us")]
		public static extern double ToMicroSeconds(uint64 ticks);

		[LinkName("ari_tm_ns")]
		public static extern double ToNanoSeconds(uint64 ticks);	}
}
