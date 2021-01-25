using System;

namespace ari.en
{
	[Component("Node")]
	public class Node 
	{
		public String Name = null ~ delete _;

		public this()
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
