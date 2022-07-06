using ari.en;
using System;

namespace testEN
{
	class Program
	{
		public static void Main()
		{
			var testQueue = new TestMpScQueue();
			testQueue.Run();
			delete testQueue;
			var n = Node.CreateComponent();
			Console.WriteLine(typeof(Node).InstanceSize);
			Console.WriteLine(n.Component.GetId());
			Test<Node>();
			Console.Read();
		}

		static ComponentHandle<IComponent> Test<T>() where T: IComponent
		{
			return T.CreateComponentGeneral();
		}
	}
}
