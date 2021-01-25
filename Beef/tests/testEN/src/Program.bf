using ari.en;
using System;

namespace testEN
{
	class Program
	{
		public static void Main()
		{
			var n = scope Node();
			Console.WriteLine(n.GetId());
			Console.Read();
		}
	}
}
