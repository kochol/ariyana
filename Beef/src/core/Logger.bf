using System;

namespace ari
{
	public static class Logger
	{
		[LinkName("ari_log_debug")]
		public static extern void Debug(char8* _text);
	}
}
