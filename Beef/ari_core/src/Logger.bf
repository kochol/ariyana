using System;

namespace ari
{
	public static class Logger
	{
		[LinkName("ari_log_debug")]
		public static extern void Debug(char8* _text);

		[LinkName("ari_log_error")]
		public static extern void Error(char8* _text);

		[LinkName("ari_log_info")]
		public static extern void Info(char8* _text);

		[LinkName("ari_log_warning")]
		public static extern void Warning(char8* _text);

	}
}
