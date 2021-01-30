using ari.en;
using System;

namespace testEN
{
	class Program
	{
		public static void Main()
		{
			var n = scope Node();
			var n2 = Node.CreateComponent();
			Console.WriteLine(typeof(Node).InstanceSize);
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
