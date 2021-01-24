using System;

namespace ari.en
{
	[Component(typeof(Node))]
	public class Node : Component<Node>
	{
		public String Name = null ~ delete _;

		protected this()
		{
		}

		public void AddChild(Node _child)
		{
		}

		public void RemoveChild(Node _child)
		{
		}
	}
}
