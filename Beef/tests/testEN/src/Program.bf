using ari.en;
using System;

namespace testEN
{
	class Program
	{
		public static void Main()
		{
			var n = scope Node();
			Console.WriteLine(sizeof(Node));
			Console.WriteLine(n.GetId());
			Test<Node>();
			Console.Read();
		}

		static void Test<T>() where T: IComponent
		{
			T.CreateComponentGeneral();
		}
	}
}
